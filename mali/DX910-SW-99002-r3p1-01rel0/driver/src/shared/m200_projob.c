/*
 * Copyright (C) 2010-2012 ARM Limited. All rights reserved.
 * 
 * This program is free software and is provided to you under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation, and any use by you of this program is subject to the terms of such GNU licence.
 * 
 * A copy of the licence is included with the program, and can also be obtained from Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <mali_system.h>
#include <shared/m200_gp_frame_builder.h>
#include <shared/m200_gp_frame_builder_struct.h>
#include <shared/m200_gp_frame_builder_inlines.h>
#include <base/pp/mali_pp_job.h>
#include <shared/m200_projob.h>
#include "m200_regs.h"

#include <regs/MALIGP2/mali_gp_vs_config.h>


#define PROJOB_DRAWCALL_WIDTH  16
#define PROJOB_DRAWCALL_HEIGHT 16
#define PROJOB_DRAWCALL_LIMIT (PROJOB_DRAWCALL_WIDTH*PROJOB_DRAWCALL_HEIGHT)



/**
 * Draw a quad onto a given master tile list. 
 *
 * This function is adds one drawcall to a pp tile list memory area. 
 *
 * This function will add 16 bytes of data to the pointer given pointer at the given offset. 
 * Sizes are passed in for safety reasons. The next 8 bytes are then listed as an "end list"
 * command. 
 *
 * @param master_tile_list_mem - A CPU memory pointer. 
 *                               Should probably point to some mapped GPU memory 
 * @param tile_list_mem_offset - The offset of the memory to place the commands onto. 
 *                               Inout parameter, will be increased by 16 bytes after calling. 
 * @param tile_list_mem_size   - The size of the master_tile_list_mem
 * @param mali_vertex_address  - The GPU address of the vertex buffer
 * @param mali_rsw_address     - The GPU address of the RSW 
 *
 * Note: There must be at least 16+8 available bytes in the master tile list mem block. 
 * An assert will trigger otherwise. 
 *
 */
MALI_STATIC void _mali200_draw_pp_job_quad( mali_tilelist_cmd*      master_tile_list_mem,
                                            u32*    /*inout*/       tile_list_mem_offset, 
                                            u32                     tile_list_mem_size,
                                            mali_addr               mali_vertex_address,
                                            mali_addr               mali_rsw_address)
{
	mali_tilelist_cmd* ptr = (mali_tilelist_cmd*) (((u8*)master_tile_list_mem) + *tile_list_mem_offset);

	MALI_DEBUG_ASSERT_POINTER(master_tile_list_mem);
	MALI_DEBUG_ASSERT(*tile_list_mem_offset + 3*sizeof(mali_tilelist_cmd) <= tile_list_mem_size, 
	                  ("Not enough room in master tile list: space=%i, need %i", tile_list_mem_size-(*tile_list_mem_offset), 3*sizeof(mali_tilelist_cmd))); 
	MALI_IGNORE(tile_list_mem_size); /* for release builds */
	MALI_PL_CMD_SET_BASE_VB_RSW(ptr[0], mali_vertex_address, mali_rsw_address);
	MALI_PL_PRIMITIVE(ptr[1], 
	                  MALI_PRIMITIVE_TYPE_PIXEL_RECT, 
					  0, /* rsw index, always equal to the base RSW at this point */
					  0, /* vertex0 index*/
					  1, /* vertex1 index*/ 
					  2  /* vertex2 index*/);
	MALI_PL_CMD_END_OF_LIST(ptr[2]);
	/* update the inout ptr */
	(*tile_list_mem_offset) += 2*sizeof(mali_tilelist_cmd);

}

MALI_STATIC mali_err_code allocate_and_setup_pp_job(struct mali_projob* projob, mali_mem_pool* pool, mali_frame_builder* frame_builder, mali_internal_frame* frame)
{
	void* outbuf;
	mali_tilelist_cmd* tilebuf;
	mali_addr out_addr;
	mali_addr tile_addr;
	void* new_jobarray;
	mali_pp_job_handle new_job;
	u32 num_tilebuf_commands;

	MALI_DEBUG_ASSERT_POINTER(projob);
	MALI_DEBUG_ASSERT_POINTER(pool);
	MALI_DEBUG_ASSERT_POINTER(frame_builder);

	/* grow PP job array size by one */
	new_jobarray = _mali_sys_realloc(projob->projob_pp_job_array,
	                  sizeof(mali_pp_job_handle) * (projob->projob_pp_job_array_size + 1));
	if(new_jobarray == NULL) return MALI_ERR_OUT_OF_MEMORY;
	/* use new array regardless of future errors, but don't grow the size until we use it*/
	projob->projob_pp_job_array = new_jobarray;

	/* allocate new empty job. Will mostly be set up on flush!
	 * We allocate this now to avoid memfail issues on flush! */
	new_job = _mali_pp_job_new(frame_builder->base_ctx, 1); /* split count 1 */ 
	if(new_job == NULL) return MALI_ERR_OUT_OF_MEMORY;

	/* we need to set up all registers for this job. The WBx registers and the 
	 * tilelist pointers in particular. Both memory blocks must be allocated right here */

	/* allocate a new output buffer. This is allocated on the fbuilder framepool. 
	 * Need space for PROJOB_DRAWCALL_LIMIT drawcalls * vec4 * fp16 bytes outputs.
	 * The space also need to be a full tile worth of outputs, so using MALI_ALIGN
	 * to expand the limit to the next MALI200_TILE_SIZE boundary. */
	outbuf = _mali_mem_pool_alloc(pool,
	             MALI_ALIGN(PROJOB_DRAWCALL_LIMIT, MALI200_TILE_SIZE*MALI200_TILE_SIZE)
				 * 4 * sizeof(u16), &out_addr);	
	if(outbuf == NULL) 
	{
		_mali_pp_job_free(new_job);
		return MALI_ERR_OUT_OF_MEMORY;
	}

	/* allocate new tilelist buffer. This is allocated on the fbuilder framepool. 
	 * Need space for PROJOB_DRAWCALL_LIMIT * 2 commands � 8 bytes each, plus initialization
	 * commands and an end command */
	num_tilebuf_commands = PROJOB_DRAWCALL_LIMIT*2 + 2; /*+2 for "begin tile" and "end list" */
	tilebuf = _mali_mem_pool_alloc(pool,
	             num_tilebuf_commands*sizeof(mali_tilelist_cmd), &tile_addr);	
	if(tilebuf == NULL) 
	{
		_mali_pp_job_free(new_job);
		return MALI_ERR_OUT_OF_MEMORY;
	}

	/* setup registers */
	_m200_frame_reg_write_common(new_job, M200_FRAME_REG_REND_LIST_ADDR, tile_addr);
	_m200_frame_reg_write_common(new_job, M200_FRAME_REG_REND_RSW_BASE, _mali_mem_mali_addr_get( frame_builder->dummy_rsw_mem, 0));
	_m200_frame_reg_write_common(new_job, M200_FRAME_REG_FEATURE_ENABLE, M200_FEATURE_FP_TILEBUF_ENABLE );
	/* Not set: Stack addr, offsets. Currently all projobs don't use stack, so it's not needed. */
	_m200_wb_reg_write( new_job, 0, M200_WBx_REG_SOURCE_SELECT, M200_WBx_SOURCE_ARGB );
	_m200_wb_reg_write( new_job, 0, M200_WBx_REG_TARGET_ADDR, out_addr );
	_m200_wb_reg_write( new_job, 0, M200_WBx_REG_TARGET_PIXEL_FORMAT, MALI_PIXEL_FORMAT_ARGB_FP16 );
	_m200_wb_reg_write( new_job, 0, M200_WBx_REG_TARGET_LAYOUT, MALI_PIXEL_LAYOUT_LINEAR);
	_m200_wb_reg_write( new_job, 0, M200_WBx_REG_TARGET_SCANLINE_LENGTH, MALI_ALIGN(PROJOB_DRAWCALL_WIDTH, MALI200_TILE_SIZE)*sizeof(u16)*4 / 8);
	_m200_wb_reg_write( new_job, 0, M200_WBx_REG_TARGET_FLAGS, M200_WBx_TARGET_FLAGS_SWAP_RED_BLUE_ENABLE);

#if MALI_TIMELINE_PROFILING_ENABLED
	_mali_pp_job_set_identity(new_job, frame_builder->identifier | MALI_FRAME_BUILDER_TYPE_PROJOB_BIT, frame->flush_id);
#else 
	MALI_IGNORE(frame);
#endif

	/* and set this new job as the current frame */
	projob->projob_pp_job_array[projob->projob_pp_job_array_size] = new_job;
	projob->projob_pp_job_array_size++;

	projob->mapped_pp_tilelist = tilebuf;
	projob->mapped_pp_tilelist_bytesize = num_tilebuf_commands*sizeof(mali_tilelist_cmd);
	projob->mapped_pp_tilelist_byteoffset = 1*sizeof(mali_tilelist_cmd); /* adding 1 command: "begin tile" */
    projob->pp_tilelist_addr = tile_addr;
    projob->output_mali_addr = out_addr;

	projob->num_pp_drawcalls_added_to_the_current_pp_job = 0;

	/* fill in begin tile / end list commands in the tile buffer */
	MALI_PL_CMD_BEGIN_NEW_TILE( tilebuf[0], 0, 0 );
	MALI_PL_CMD_END_OF_LIST(tilebuf[1]);

	return MALI_ERR_NO_ERROR;
}


mali_addr _mali_projob_add_pp_drawcall(struct mali_frame_builder* frame_builder, mali_addr rsw_address)
{
	mali_mem_pool* pool;
	int xoffset = 0;
	int yoffset = 0;
	float* posbuf;
	mali_addr pos_addr = 0;
	mali_addr out_addr = 0;
	
	struct mali_projob* projob;
	mali_internal_frame* frame;

	MALI_DEBUG_ASSERT_POINTER(frame_builder);

	frame = GET_CURRENT_INTERNAL_FRAME(frame_builder);

	MALI_DEBUG_ASSERT_POINTER(frame);

	projob = &frame->projob;

	MALI_DEBUG_ASSERT(frame->state == FRAME_DIRTY, ("Current frame must be in the DIRTY state"));

	pool = _mali_frame_builder_frame_pool_get( frame_builder);

	/* If there is no current job or no more space in the current job, 
	 * reallocate room for a new HW job, and a new HW job */
	if(projob->num_pp_drawcalls_added_to_the_current_pp_job >= PROJOB_DRAWCALL_LIMIT ||
	   projob->projob_pp_job_array == NULL ||
       projob->projob_pp_job_array_size == 0 ||
       projob->projob_pp_job_array[projob->projob_pp_job_array_size -1] == NULL) /* no current job */
	{
		mali_err_code err = allocate_and_setup_pp_job(projob, pool, frame_builder, frame);
		if(err != MALI_ERR_NO_ERROR) return 0;
	}
	   
	/* figure out where the new drawcall should reside, based on num_drawcalls_added 
	 * The current code assumes a 16*16 tile, all drawcalls are added in order. 
	 * The output address for a projob is on the output buffer, offseted for pixel coord.*/
	xoffset = projob->num_pp_drawcalls_added_to_the_current_pp_job % PROJOB_DRAWCALL_WIDTH;
	yoffset = projob->num_pp_drawcalls_added_to_the_current_pp_job / PROJOB_DRAWCALL_WIDTH;
	MALI_DEBUG_ASSERT(xoffset < PROJOB_DRAWCALL_WIDTH, ("Projob pixel must be inside the screen"));
	MALI_DEBUG_ASSERT(yoffset < PROJOB_DRAWCALL_HEIGHT, ("Projob pixel must be inside the screen"));

	out_addr = (projob->output_mali_addr) + 
	           (MALI_ALIGN(PROJOB_DRAWCALL_WIDTH, MALI200_TILE_SIZE) * 4 * sizeof(u16) * yoffset) +
	           4 * sizeof(u16) * xoffset;


	/* allocate position buffer. It need to hold 3 vertices for a standard quad, each
	 * a vec4 fp32 value. Then fill it in so that it is covering the pixel in question. */
	posbuf = _mali_mem_pool_alloc(pool, 3 * 4 * sizeof(float), &pos_addr);
	if(posbuf == NULL) return 0;

    posbuf[0] = xoffset + 1.0;
	posbuf[1] = yoffset;
	posbuf[2] = 0.0;
	posbuf[3] = 1.0;

	posbuf[4] = xoffset;
	posbuf[5] = yoffset;
	posbuf[6] = 0.0;
	posbuf[7] = 1.0;

	posbuf[8] = xoffset;
	posbuf[9] = yoffset + 1.0;
	posbuf[10] = 0.0;
	posbuf[11] = 1.0;

	/* and add this drawcall to the current job! */
	_mali200_draw_pp_job_quad( projob->mapped_pp_tilelist,
	                           &projob->mapped_pp_tilelist_byteoffset,
	                           projob->mapped_pp_tilelist_bytesize,
	                           pos_addr,
	                           rsw_address);

	projob->num_pp_drawcalls_added_to_the_current_pp_job++;


	return out_addr;
}


u32 _mali_projob_add_gp_drawcall(struct mali_frame_builder* frame_builder, u64* cmnds, struct bs_shaderpass* projob_shader)
{

	mali_mem_pool* pool;
	u32 *streams;
	mali_addr streams_mem;
	u32 num_cmnds = 0;
	int num_instructions, i;
	
	/* We want to set up a set of input and output streams for the pilot job, but pilot jobs doesn't use GP inputs nor outputs. 
	 * So all streams should be disabled. Unfortunately, the HW cannot encode "0 active streams", so we need at least one
	 * active stream created in both input and output. This stream must be of type GP_VS_VSTREAM_FORMAT_NO_DATA.
	 *
	 * We can then re-use that stream table for both input and output streams. 
	 *
	 */
	const int stream_setup_mem_size = 8; /* 1 stream * 64 bits per stream (32bit pointer, 32bit specifier). We will reuse this for both input and output. */

	MALI_DEBUG_ASSERT_POINTER(frame_builder);
	MALI_DEBUG_ASSERT_POINTER( projob_shader );

	pool = _mali_frame_builder_frame_pool_get( frame_builder);

	/* need a buffer for dummy  streams */
	streams = _mali_mem_pool_alloc(pool, stream_setup_mem_size, &streams_mem);
	if (NULL == streams) return 0;

	streams[ GP_VS_CONF_REG_INP_ADDR( 0 ) ] = _SWAP_ENDIAN_U32_U32(streams_mem); /* points to itself. No data will be read, but the pointer must be legal */
	streams[ GP_VS_CONF_REG_INP_SPEC( 0 ) ] = _SWAP_ENDIAN_U32_U32(GP_VS_VSTREAM_FORMAT_NO_DATA);

	/* put all the projob passes into the VS CMD list */

	/* dummy streams, put the same memory for input/output, we don't care about this memory assignment*/
	cmnds[num_cmnds++] = GP_VS_COMMAND_WRITE_INPUT_OUTPUT_CONF_REGS(streams_mem, 0, 1);  /* setup 1 input stream */
	cmnds[num_cmnds++] = GP_VS_COMMAND_WRITE_INPUT_OUTPUT_CONF_REGS(streams_mem, 1, 1);  /* setup 1 output stream */

	num_instructions = projob_shader->flags.vertex.instruction_end;

	/* set vertex program parameters */
	cmnds[num_cmnds++] = GP_VS_COMMAND_LOAD_SHADER(
				_mali_mem_mali_addr_get(projob_shader->shader_binary_mem->mali_memory, 0), /* shader address */
				0,                                                                         /* destination instruction */
				num_instructions                                                           /* length */
				);

	cmnds[num_cmnds++] = GP_VS_COMMAND_WRITE_CONF_REG( GP_VS_CONF_REG_PROG_PARAM_CREATE(projob_shader->flags.vertex.instruction_start, 
	                                                                                    projob_shader->flags.vertex.instruction_end - 1, 
	                                                                                    projob_shader->flags.vertex.instruction_last_touching_input - 1), 
	                                                                                    GP_VS_CONF_REG_PROG_PARAM);
	cmnds[num_cmnds++] = GP_VS_COMMAND_WRITE_CONF_REG( GP_VS_CONF_REG_OPMOD_CREATE(1, 1), GP_VS_CONF_REG_OPMOD ); /* 1 input, 1 output */

	
	/* shade vertices : operation mode, number_of_verts = 1 */
	cmnds[num_cmnds++] = GP_VS_COMMAND_SHADE_VERTICES( GP_PLBU_OPMODE_DRAW_ELEMENTS, 1 );
	cmnds[num_cmnds++] = GP_VS_COMMAND_FLUSH_WRITEBACK_BUF();

	return num_cmnds;

}

/**************** PUBLIC FUNCTIONS **********************/

void _mali_projob_pp_flush(mali_internal_frame* frame)
{
	u32 i;
	struct mali_projob* projob;
	mali_pp_job_handle* pp_job_array;
    u32 pp_job_array_size;

	MALI_DEBUG_ASSERT_POINTER(frame);
	MALI_DEBUG_ASSERT_POINTER(&frame->projob);

	projob = &frame->projob;

	/* early out? */
	if(0 == projob->num_pp_drawcalls_added_to_the_current_pp_job) return;

	/* problem: starting a HW job can cause it to finish instantly. 
	 * Finishing a job can cause it to be cleaned up, doing callbacks. 
	 *
	 * There is as such always a risk that calling _mali_pp_job_start
	 * may lead to a series of calls that eventually calls
	 * _mali_projob_pp_reset. And if not from the same thread, 
	 * there is always the change of various race conditions. 
	 * 
	 * To be safe, we need to store off anything from the projob struct onto
	 * local stack, clean up the projob struct so that it no longer reference
	 * these values, and then call _mali_pp_job_start on the jobs. 
	 */
	pp_job_array = projob->projob_pp_job_array;
	projob->projob_pp_job_array = NULL;
	pp_job_array_size = projob->projob_pp_job_array_size;
	projob->projob_pp_job_array_size = 0;

	/* now that the struct is clean, call all the local PP jobs, 
	 * then clean up the stack variables we just set up. */

	for( i = 0; i < pp_job_array_size; i++)
	{
		MALI_DEBUG_ASSERT_POINTER(pp_job_array);
		MALI_DEBUG_ASSERT_POINTER(pp_job_array[i]);
		_mali_pp_job_start(pp_job_array[i], MALI_JOB_PRI_NORMAL);
	}

	/* since there is draw calls added, there is also at least one job using it.
	 * Though it is possible that we failed to allocate it. So add an if here 
	 * for safety. */
	if(pp_job_array) _mali_sys_free(pp_job_array);
	
}

void _mali_projob_reset(mali_internal_frame* frame)
{
	u32 i;
	struct mali_projob* projob;
	mali_pp_job_handle* pp_job_array;
    u32 pp_job_array_size;
	
	MALI_DEBUG_ASSERT_POINTER(frame);
	MALI_DEBUG_ASSERT_POINTER(&frame->projob);

	projob = &frame->projob;

	/* early out? */
	if(0 == projob->num_pp_drawcalls_added_to_the_current_pp_job) return;

	/* problem: like with flushing, freeing a HW job can cause
	 * a series of callbacks that call reset. Effectively we end up
	 * calling reset within reset. That's messy.
	 *
	 * To be safe, we need to store off anything from the projob struct onto
	 * local stack, clean up the projob struct so that it no longer reference
	 * these values, and then call _mali_pp_job_free on the jobs. 
	 */
	pp_job_array = projob->projob_pp_job_array;
	projob->projob_pp_job_array = NULL;
	pp_job_array_size = projob->projob_pp_job_array_size;
	projob->projob_pp_job_array_size = 0;
	
	for( i = 0; i < pp_job_array_size; i++)
	{
		MALI_DEBUG_ASSERT_POINTER(pp_job_array);
		MALI_DEBUG_ASSERT_POINTER(pp_job_array[i]);
		_mali_pp_job_free(pp_job_array[i]);
	}

	/* since there is draw calls added, there is also at least one job using it.
	 * Though it is possible that we failed to allocate it. So add an if here 
	 * for safety. */
	if(pp_job_array) _mali_sys_free(pp_job_array);
	
}


mali_bool _mali_projob_have_drawcalls(struct mali_internal_frame* frame)
{
	struct mali_projob* projob;
	MALI_DEBUG_ASSERT_POINTER(frame);
	MALI_DEBUG_ASSERT_POINTER(&frame->projob);
	projob = &frame->projob;

	return (0 < projob->num_pp_drawcalls_added_to_the_current_pp_job);
}
