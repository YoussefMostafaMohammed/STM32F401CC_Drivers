#ifndef BIT_MATH_H_
#define BIT_MATH_H_

#define SET_BIT(REG,BIT) ((REG) |= (1U << (BIT)))

#define CLR_BIT(REG,BIT) ((REG) &= ~(1U << (BIT)))

#define TGL_BIT(REG,BIT) ((REG) ^= (1U << (BIT)))

#define GET_BIT(REG,BIT) (((REG) >> (BIT)) & 1U)

#define SET_HIGH_NIB(REG) ((REG) |= (0xF0))

#define CLR_HIGH_NIB(REG) ((REG) &= ~(0xF0))

#define GET_HIGH_NIB(REG) (((REG) & (0xF0)) >> 4)

#define TGL_HIGH_NIB(REG) ((REG) ^= (0xF0))

#define SET_LOW_NIB(REG) ((REG) |= (0x0F))

#define CLR_LOW_NIB(REG) ((REG) &= ~(0x0F))

#define GET_LOW_NIB(REG) ((REG) & (0x0F))

#define TGL_LOW_NIB(REG) ((REG) ^= (0x0F))

#define ASSIGN_REG(REG,VALUE) ((REG) = (VALUE))

#define ASSIGN_HIGH_NIB(REG,VALUE) ((REG) = ((REG) & ~(0xF0)) | (((VALUE) & 0x0F) << 4))

#define ASSIGN_LOW_NIB(REG,VALUE) ((REG) = ((REG) & ~(0x0F)) | ((VALUE) & 0x0F))

#define RSHFT_REG(REG, NO) ((REG) >>= (NO))

#define LSHFT_REG(REG, NO) ((REG) <<= (NO))

#define CRSHFT_REG(REG, NO) ((REG) = ((REG) >> (NO)) | ((REG) << (8 * sizeof(REG) - (NO))))

#define CLSHFT_REG(REG, NO) ((REG) = ((REG) << (NO)) | ((REG) >> (8 * sizeof(REG) - (NO))))

#endif