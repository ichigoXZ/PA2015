#include "common.h"
#include "burst.h"
#include "misc.h"

#define WAY_WIDTH 3
#define ROW_WIDTH 7
#define BLOCK_WIDTH 6
#define REM_WIDTH (27 - WAY_WIDTH - ROW_WIDTH - BLOCK_WIDTH)

typedef union{
	struct 
	{
		uint32_t way 	: WAY_WIDTH;
		uint32_t row 	: ROW_WIDTH;
		uint32_t block 	: BLOCK_WIDTH;
		uint32_t rem 	: REM_WIDTH;
	};
	uint32_t addr;
} cache_addr;

#define NR_WAY (1 << WAY_WIDTH)
#define NR_ROW (1 << ROW_WIDTH)
#define NR_BLOCK (1 << BLOCK_WIDTH)
#define NR_REM (1 << REM_WIDTH)