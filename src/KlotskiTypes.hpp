#ifndef _KLOTSKITYPES_HPP_
#define _KLOTSKITYPES_HPP_

#define BIT0 (1)
#define BIT1 (2)
#define BIT2 (4)
#define BIT3 (8)
#define BIT4 (16)
#define BIT5 (32)
#define BIT6 (64)
#define BIT7 (128)

#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lParam)    ((int)(short)LOWORD(lParam))
#endif
#ifndef GET_Y_LPARAM
#define GET_Y_LPARAM(lParam)    ((int)(short)HIWORD(lParam))
#endif

typedef struct _StepInfo
{
    char id;
    char move;
} StepInfo;

typedef enum _Direction
{
	UP='U',
	DOWN='D',
	LEFT='L',
	RIGHT='R',
	ERR='#'
} Direction;

typedef struct _KlotskiMapInfo
{
    int width;
    int height;
    char *mapdata;
    char *mapname;
}KlotskiMapInfo;

#endif
