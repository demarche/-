#include "DxLib.h"
#include <time.h>

// ピースの横と縦のマス数
#define PIECE_WIDTH     4
#define PIECE_HEIGHT    4

/* フィールドの横と縦のマス数 */
#define FIELD_WIDTH     24
#define FIELD_HEIGHT    25

// マスのピクセル数
#define CELL_WIDTH      18
#define CELL_HEIGHT     18

//ホールド可能な数
#define CAN_HOLD		2

BYTE field[FIELD_WIDTH][FIELD_HEIGHT]={0};      /* ゲームフィールド */
DWORD fColor[FIELD_WIDTH][FIELD_HEIGHT]={0};    // ゲームフィールドの色

BYTE piece[PIECE_WIDTH][PIECE_HEIGHT]={0};      /* 現在移動中のブロック */
DWORD pColor[PIECE_WIDTH][PIECE_HEIGHT]={0};    // 現在移動中のブロックの色
POINT location={0,0};                           /* 現在移動中のブロックの位置 */

BYTE next[PIECE_WIDTH][PIECE_HEIGHT]={0};       // 次のブロック
DWORD nColor[PIECE_WIDTH][PIECE_HEIGHT]={0};    // 次のブロックの色

BYTE hold[CAN_HOLD][PIECE_WIDTH][PIECE_HEIGHT]={0};       // ホールド中のブロック1
DWORD hColor[CAN_HOLD][PIECE_WIDTH][PIECE_HEIGHT]={0};    // ホールド中のブロックの色1


bool FirstHold[CAN_HOLD] = {true,true};
BOOL GameOver = FALSE;

int difficult = 0;


void CreatePiece(void)
{
    for(int y=0;y<PIECE_HEIGHT;y++){
        for(int x=0;x<PIECE_WIDTH;x++){
            next[x][y]=0;
            nColor[x][y]=0;
        }
    }
	int DifofPiece = 7;

	 srand((unsigned) time(NULL));
	if(difficult<4||difficult==9){
		switch(rand()%DifofPiece){
			case 0:
				next[1][1]=1; next[2][1]=1; next[1][2]=1; next[2][2]=1;
				nColor[1][1]=nColor[2][1]=nColor[1][2]=nColor[2][2]=0x000000ff;
				return;
			case 1:
				next[1][0]=1; next[1][1]=1; next[1][2]=1; next[1][3]=1;
				nColor[1][0]=nColor[1][1]=nColor[1][2]=nColor[1][3]=0x0000ff00;
				return;
			case 2:
				next[1][1]=1; next[1][2]=1; next[2][2]=1; next[1][3]=1;
				nColor[1][1]=nColor[1][2]=nColor[2][2]=nColor[1][3]=0x0000ffff;
				return;
			case 3:
				next[1][1]=1; next[2][1]=1; next[1][2]=1; next[1][3]=1;
				nColor[1][1]=nColor[2][1]=nColor[1][2]=nColor[1][3]=0x00ff0000;
				return;
			case 4:
				next[1][1]=1; next[2][1]=1; next[2][2]=1; next[2][3]=1;
				nColor[1][1]=nColor[2][1]=nColor[2][2]=nColor[2][3]=0x00ff00ff;
				return;
			case 5:
				next[2][1]=1; next[1][2]=1; next[2][2]=1; next[1][3]=1;
				nColor[2][1]=nColor[1][2]=nColor[2][2]=nColor[1][3]=0x00ffff00;
				return;
			case 6:
				next[1][1]=1; next[1][2]=1; next[2][2]=1; next[2][3]=1;
				nColor[1][1]=nColor[1][2]=nColor[2][2]=nColor[2][3]=0x00ffffff;
				return;
			case 7:
				if(difficult>=3){
					srand((unsigned) time(NULL));
					for(int y=0;y<PIECE_HEIGHT;y++){
						for(int x=0;x<PIECE_WIDTH;x++){
							next[x][y]=rand()%2;
							nColor[x][y]=(rand()*rand()*rand())%0xffffffff;
						}
					}
				}else{
					srand((unsigned) time(NULL));
					int basex=rand()%PIECE_WIDTH,basey=rand()%PIECE_HEIGHT;

					next[basex][basey]=1;

					for(int i=0;i<3;i++){
						int muki=rand()%4;

						switch(muki){
						case 0:
							if(basex+1>=PIECE_WIDTH)continue;
							basex++;
							break;
						case 1:
							if(basex-1<0)continue;
							basex--;
							break;
						case 2:
							if(basey+1>=PIECE_HEIGHT)continue;
							basey++;
							break;
						case 3:
							if(basey-1<0)continue;
							basey--;
							break;

						}
						next[basex][basey]=1;
					}
					/*for(int y=rand()%2;y<PIECE_HEIGHT-(rand()%2);y++){
						for(int x=rand()%2;x<PIECE_WIDTH-(rand()%2);x++){
							next[x][y]=rand()%2;
							nColor[x][y]=(rand()*rand()*rand())%0xffffffff;
						}
					}*/
				}
				return;
			/*case 8:
				srand((unsigned) time(NULL));
				for(int y=0;y<PIECE_HEIGHT;y++){
					for(int x=0;x<PIECE_WIDTH;x++){
						next[x][y]=rand()%2;
						nColor[x][y]=(rand()*rand()*rand())%0xffffffff;
					}
				}*/
			case 9:
				next[1][1]=1;
				nColor[1][1]=0xffffffff;
				return;
			case 10:
				for(int y=0;y<PIECE_HEIGHT;y++){
					for(int x=0;x<PIECE_WIDTH;x++){
						next[x][y]=1;
						nColor[x][y]=1;
					}
				}
				nColor[1][1]=0x00000000;
				return;
		}
	}else if(difficult>=4&&difficult<=6){
		srand((unsigned) time(NULL));
					int basex=rand()%PIECE_WIDTH,basey=rand()%PIECE_HEIGHT , baseCol=(rand()*rand())%0x100000000;

					next[basex][basey]=1;
					nColor[basex][basey]=baseCol;

					int loop=0;
					for(int i=0;i<difficult;i++){
						int muki=rand()%4;
						loop++;

						if(loop>100)break;

						switch(muki){
						case 0:
							if(basex+1>=PIECE_WIDTH||next[basex+1][basey]==1){
								i--;
								continue;
							}
							basex++;
							break;
						case 1:
							if(basex-1<0||next[basex-1][basey]==1){
								i--;
								continue;
							}
							basex--;
							break;
						case 2:
							if(basey+1>=PIECE_HEIGHT||next[basex][basey+1]==1){
								i--;
								continue;
							}
							basey++;
							break;
						case 3:
							if(basey-1<0||next[basex][basey-1]==1){
								i--;
								continue;
							}
							basey--;
							break;

						}
						loop=0;
						next[basex][basey]=1;
						nColor[basex][basey]=baseCol;
					}
		
	}else if(difficult==7){
		srand((unsigned) time(NULL));
			for(int y=rand()%2;y<PIECE_HEIGHT-(rand()%2);y++){
				for(int x=rand()%2;x<PIECE_WIDTH-(rand()%2);x++){
					next[x][y]=rand()%2;
					nColor[x][y]=(rand()*rand()*rand())%0xffffffff;
				}
			}
	}else if(difficult==8){
		srand((unsigned) time(NULL));
		for(int y=0;y<PIECE_HEIGHT;y++){
			for(int x=0;x<PIECE_WIDTH;x++){
				next[x][y]=rand()%2;
				nColor[x][y]=(rand()*rand()*rand())%0xffffffff;
			}
		}
	}
}

void NextPiece(BOOL first)    // first：ゲーム開始から最初の呼び出しか否か
{
    if(first) CreatePiece();

    for(int y=0;y<PIECE_HEIGHT;y++){
        for(int x=0;x<PIECE_WIDTH;x++){
            piece[x][y]=next[x][y];
            pColor[x][y]=nColor[x][y];
        }
    }
    location.x=5;
    location.y=-3;

    CreatePiece();
}

// エラー
#define ERR -1

/* piece[][] 内のブロックの最上部の位置を返す */
int GetPieceTop(void)
{
    for(int y=0;y<PIECE_HEIGHT;y++){
        for(int x=0;x<PIECE_WIDTH;x++){
            if(piece[x][y]){
                return y;
            }
        }
    }
    return ERR;
}

/* piece[][]　内のブロックの最上部の位置を返す */
int GetPieceUpper(void)
{
    for(int y=0;y<PIECE_HEIGHT;y++){
        for(int x=0;x<PIECE_WIDTH;x++){
            if(piece[x][y]){
                return y;
            }
        }
    }
    return ERR;
}

/* piece[][]　内のブロックの最下部の位置を返す */
int GetPieceBottom(void)
{
    for(int y=PIECE_HEIGHT-1;y>=0;y--){
        for(int x=0;x<PIECE_WIDTH;x++){
            if(piece[x][y]){
                return y;
            }
        }
    }
    return ERR;
}

/* piece[][]　内のブロックの左側の位置を返す */
int GetPieceLeft(void)
{
    for(int x=0;x<PIECE_WIDTH;x++){
        for(int y=0;y<PIECE_HEIGHT;y++){
            if(piece[x][y]){
                return x;
            }
        }
    }
    return ERR;
}

/* piece[][]　内のブロックの右側の位置を返す */
int GetPieceRight(void)
{
    for(int x=PIECE_WIDTH-1;x>=0;x--){
        for(int y=0;y<PIECE_HEIGHT;y++){
            if(piece[x][y]){
                return x;
            }
        }
    }
    return ERR;
}

/* MovePiece 関数の引数 */
#define PIECE_LEFT              2
#define PIECE_RIGHT             4
#define PIECE_DOWN              8
#define PIECE_UP              16


BOOL MovePiece(int move)        
{
    int x,y,left,right,bottom,upper;
    switch(move){
        case PIECE_LEFT:
            left=GetPieceLeft();

            if((location.x)+left <= 0) return FALSE;

            for(y=0;y<PIECE_HEIGHT;y++){   
                for(x=0;x<PIECE_WIDTH;x++){ 
                    if(piece[x][y] && (location.x)+x-1>=0 && (location.y)+y>=0
                        && field[(location.x)+x-1][(location.y)+y]){    // 一つ左にブロックがある
                            return FALSE;
                    }
                }
            }
            location.x--;
            return TRUE;
        case PIECE_RIGHT:
            right=GetPieceRight();

            if((location.x)+right >= FIELD_WIDTH-1) return FALSE;

            for(y=0;y<PIECE_HEIGHT;y++){  
                for(x=0;x<PIECE_WIDTH;x++){ 
                    if(piece[x][y] && (location.x)+x+1<FIELD_WIDTH && (location.y)+y>=0
                        && field[(location.x)+x+1][(location.y)+y]){    // 一つ右にブロックがある
                            return FALSE;
                    }
                }
            }
            location.x++;
            return TRUE;
        case PIECE_DOWN:
            bottom=GetPieceBottom();

            if((location.y)+bottom >= FIELD_HEIGHT-1) return FALSE;

            for(y=0;y<PIECE_HEIGHT;y++){   
                for(x=0;x<PIECE_WIDTH;x++){ 
                    if(piece[x][y] && (location.y)+y+1>=0 && (location.y)+y+1<FIELD_HEIGHT
                        && field[(location.x)+x][(location.y)+y+1]){    // 一つ下にブロックがある
                            return FALSE;
                    }
                }
            }
            location.y++;
            return TRUE;
		case PIECE_UP:
            upper=GetPieceUpper();

            if((location.y)+upper < 0) return FALSE;

            for(y=0;y<PIECE_HEIGHT;y++){    
                for(x=0;x<PIECE_WIDTH;x++){  
                    if(piece[x][y] && (location.x)+x+1<FIELD_WIDTH && (location.y)+y>=0
                        && field[(location.x)-x][(location.y)-y]){    // 一つ上にブロックがある
                            return FALSE;
                    }
                }
            }
            location.y--;
            return TRUE;
    }
    return FALSE;
}



BOOL TurnPiece(void)
{
    int x,y,offsetX,offsetY;
    BYTE pTurn[PIECE_WIDTH][PIECE_HEIGHT];
    DWORD pcTurn[PIECE_WIDTH][PIECE_HEIGHT];


    for(y=0;y<PIECE_HEIGHT;y++){
        for(x=0;x<PIECE_WIDTH;x++){
            pTurn[(PIECE_HEIGHT-1)-y][x]=piece[x][y];
            pcTurn[(PIECE_HEIGHT-1)-y][x]=pColor[x][y];
        }
    }

    for(y=0;y<PIECE_HEIGHT;y++){
        for(x=0;x<PIECE_WIDTH;x++){
            if(pTurn[x][y]){
                offsetX=(location.x)+x;
                offsetY=(location.y)+y;
                if(offsetX<0 || offsetX>=FIELD_WIDTH
                    || offsetY>=FIELD_HEIGHT 
                    || (offsetY>=0 && field[offsetX][offsetY])){    //既にブロックがある
                        return FALSE;
                }
            }
        }
    }
    for(y=0;y<PIECE_HEIGHT;y++){
        for(x=0;x<PIECE_WIDTH;x++){
            piece[x][y]=pTurn[x][y];
            pColor[x][y]=pcTurn[x][y];
        }
    }
    return TRUE;
}

void drawBlock(int x,int y,int col){
	x-=CELL_WIDTH;
	DrawTriangle( x, y, x, y+CELL_HEIGHT,x+CELL_WIDTH-1, y, 0xffffffff ,TRUE) ;
	DrawTriangle( x+CELL_WIDTH, y+CELL_HEIGHT, x, y+CELL_HEIGHT-1,x+CELL_WIDTH, y, 0x00000000 , TRUE) ;
	DrawBox(x+2,y+2,x+CELL_WIDTH-2,y+CELL_HEIGHT-2,col, TRUE ) ; 
}

void drawTransBlock(int x,int y,int col){
	x-=CELL_WIDTH;
	int trans = 0x44444444;
	DrawTriangle( x, y, x, y+CELL_HEIGHT,x+CELL_WIDTH-1, y, 0xffffffff-trans ,TRUE) ;
	DrawTriangle( x+CELL_WIDTH, y+CELL_HEIGHT, x, y+CELL_HEIGHT-1,x+CELL_WIDTH, y, 0x00000000+trans , TRUE) ;
	DrawBox(x+2,y+2,x+CELL_WIDTH-2,y+CELL_HEIGHT-2,col/7*3, TRUE ) ; 
}

void HoldBlock(int num){

	BYTE tmp[PIECE_WIDTH][PIECE_HEIGHT]={0};      
	DWORD tmpColor[PIECE_WIDTH][PIECE_HEIGHT]={0}; 

	for(int y=0;y<PIECE_HEIGHT;y++){
        for(int x=0;x<PIECE_WIDTH;x++){
			tmp[x][y]=hold[num][x][y];
			tmpColor[x][y]=hColor[num][x][y];
        }
    }

	for(int y=0;y<PIECE_HEIGHT;y++){
        for(int x=0;x<PIECE_WIDTH;x++){
			hold[num][x][y]=piece[x][y];
            hColor[num][x][y]=pColor[x][y];
        }
    }
	if(FirstHold[num]==true){
		FirstHold[num]=false;
		NextPiece(FALSE);

	}else{
		for(int y=0;y<PIECE_HEIGHT;y++){
			for(int x=0;x<PIECE_WIDTH;x++){
				piece[x][y]=tmp[x][y];
				pColor[x][y]=tmpColor[x][y];
			}
		}
		/*location.x=5;
		location.y=-3;*/

		//CreatePiece();
	}
}

void paint(){
	int x,y,ptx,pty,btx,bty,tmpy=0;
	for(int x=0;x<32;x++){//フィールド
		for(int y=0;y<28;y++){
			if((x>0&&x<=FIELD_WIDTH)&&y<FIELD_HEIGHT){
				DrawBox(x*CELL_HEIGHT,y*CELL_HEIGHT,x*CELL_HEIGHT+CELL_WIDTH,y*CELL_HEIGHT+CELL_HEIGHT,0x33333333,TRUE);
			}
		}
	}

	for(int x=0;x<32;x++){//次のピースなど
		for(int y=0;y<24;y++){
			if(x>FIELD_WIDTH+2&&x<FIELD_WIDTH+7&&((y>1&&y<6)||(y>7&&y<12)||(y>13&&y<18))){
				DrawBox(x*CELL_HEIGHT,y*CELL_HEIGHT,x*CELL_HEIGHT+CELL_WIDTH,y*CELL_HEIGHT+CELL_HEIGHT,0x33333333,TRUE);
			}
		}
	}

   
	int bottom=GetPieceBottom();
	for(int i=0;i<FIELD_HEIGHT+2;i++){
		
        if((i)+bottom >= FIELD_HEIGHT-1){
			goto end;
		}
		for(y=0;y<PIECE_HEIGHT;y++){   
                for(x=0;x<PIECE_HEIGHT;x++){ 
                    if(location.y<0&&piece[x][y] && (i)+y+1>=0 && (i)+y+1<FIELD_HEIGHT
                        && field[(location.x)+x][y+location.y+3+i+1]){    // 一つ下にブロックがある
							tmpy+=location.y+3;
							if(tmpy>=FIELD_HEIGHT-1){
								tmpy=FIELD_HEIGHT-3;
							}
                            goto end;
                    }else if(piece[x][y] && (i)+y+1>=0 && (i)+y+1<FIELD_HEIGHT
                        && field[(location.x)+x][y+location.y+i+1]){    // 一つ下にブロックがある
							tmpy+=location.y;
							if(tmpy>=FIELD_HEIGHT-1){
								tmpy=FIELD_HEIGHT;
							}
                            goto end;
                    }
                }
         }
		
	
         tmpy++;
	}
end:
	
	for(y=0;y<PIECE_HEIGHT;y++){            // 予測されたブロック
        //if((location.y)+y < 0) continue;    // ゲームフィールドの枠より上は描かない
        for(x=0;x<PIECE_WIDTH;x++){
            btx=((location.x)+x+2)*CELL_WIDTH;
            bty=((tmpy)+y)*CELL_HEIGHT;
            if(piece[x][y]){
				drawTransBlock(btx,bty,pColor[x][y]);
				
            }
        }
    }

	for(int x=0;x<32;x++){//フィールド下部
		for(int y=0;y<24;y++){
			if((x>0&&x<=FIELD_WIDTH)&&y==FIELD_HEIGHT){
				DrawBox(x*CELL_HEIGHT,y*CELL_HEIGHT,x*CELL_HEIGHT+CELL_WIDTH,y*CELL_HEIGHT+CELL_HEIGHT,0x00000000,TRUE);
			}
		}
	}

	for(y=0;y<PIECE_HEIGHT;y++){            // 現在移動中のブロック
        //if((location.y)+y < 0) continue;    // ゲームフィールドの枠より上は描かない
        for(x=0;x<PIECE_WIDTH;x++){
            ptx=((location.x)+x+2)*CELL_WIDTH;
            pty=((location.y)+y)*CELL_HEIGHT;
            if(piece[x][y]){
                drawBlock(ptx,pty,pColor[x][y]);
				
            }
        }
    }
    
	 for(y=0;y<FIELD_HEIGHT;y++){    // ゲームフィールドのブロック
        for(x=0;x<FIELD_WIDTH;x++){
            ptx=(x+2)*CELL_WIDTH;
            pty=(y)*CELL_HEIGHT;
            if(field[x][y]){
				drawBlock(ptx,pty,fColor[x][y]); 
            }
        }
    }
	for(y=0;y<PIECE_HEIGHT;y++){    // 次のブロック
        for(x=0;x<PIECE_WIDTH;x++){
            ptx=(FIELD_WIDTH+x+4)*CELL_WIDTH;
            pty=(y+2)*CELL_HEIGHT;
            if(next[x][y]){
				drawBlock(ptx,pty,nColor[x][y]);
            }
        }
    }
	
	for(int i=0;i<CAN_HOLD;i++){
		for(y=0;y<PIECE_HEIGHT;y++){    // ホールド中のブロック
			for(x=0;x<PIECE_WIDTH;x++){
				ptx=(FIELD_WIDTH+x+4)*CELL_WIDTH;
				pty=(y+8+(i*6))*CELL_HEIGHT;
				if(hold[i][x][y]){
					drawBlock(ptx,pty,hColor[i][x][y]);
				}
			}
		}
	}

	

}

int DeleteLine(void)
{
    int x,y,delCount=0;
    for(y=FIELD_HEIGHT-1;y>=0;y--){
        int lineCount=0;
        for(x=0;x<FIELD_WIDTH;x++){
            lineCount+=field[x][y];
        }

        if(lineCount==0) break;    /* これより上にブロックはない */
        if(lineCount!=FIELD_WIDTH) continue;

        /* 一行削除する */
        delCount++;
        for(x=0;x<FIELD_WIDTH;x++){
            field[x][y]=0;
        }
    }
    return delCount;
}
// 削除した行を詰める
void ShiftLine(int delCount) 
{
    int x,y;
    for(y=FIELD_HEIGHT-1;y>=0 && delCount>0; ){
        int lineCount=0;
        for(x=0;x<FIELD_WIDTH;x++){
            lineCount+=field[x][y];
        }

        if(lineCount!=0){
            y--;
            continue;
        }

        // 一行詰める
        delCount--;
        for(int iy=y;iy>=0;iy--){
            for(x=0;x<FIELD_WIDTH;x++){
                if(iy-1>=0){
                    field[x][iy]=field[x][iy-1];
                    fColor[x][iy]=fColor[x][iy-1];
                }else{
                    field[x][0]=0;  
                    fColor[x][0]=0;
                }
            }
        }
    }
}

/* ブロックを位置情報に従ってフィールドにコピーする */
void PieceToField(void)
{
	if(difficult==9){
		BYTE tmp1[PIECE_WIDTH][PIECE_HEIGHT]={0}; 
		DWORD tmp1Col[PIECE_WIDTH][PIECE_HEIGHT]={0};
		BYTE tmp2[PIECE_WIDTH][PIECE_HEIGHT]={0}; 
		DWORD tmp2Col[PIECE_WIDTH][PIECE_HEIGHT]={0};

		for(int y=0;y<PIECE_HEIGHT;y++){//ブロックの左側コピー
			for(int x=0;x<PIECE_WIDTH/2;x++){    
				if(piece[x][y] && (location.y)+y>=0){
					tmp1[x][y]=piece[x][y];
					tmp1Col[x][y]=pColor[x][y];
				}
			}
		}

		for(int y=0;y<PIECE_HEIGHT;y++){//ブロックの右側コピー
			for(int x=PIECE_WIDTH/2;x<PIECE_WIDTH;x++){    
				if(piece[x][y] && (location.y)+y>=0){
					tmp2[x][y]=piece[x][y];
					tmp2Col[x][y]=pColor[x][y];
				}
			}
		}

		for(int y=0;y<PIECE_HEIGHT;y++){//左側移動
			for(int x=0;x<PIECE_WIDTH;x++){    
				piece[x][y]=tmp1[x][y];
				pColor[x][y]=tmp1Col[x][y];
			}
		}
		while(MovePiece(PIECE_LEFT)==TRUE){
			Sleep(1);
		}

		for(int y=0;y<PIECE_HEIGHT;y++){
			for(int x=0;x<PIECE_WIDTH;x++){    
				if(piece[x][y] && (location.y)+y>=0){
					field[(location.x)+x][(location.y)+y]=piece[x][y];
					fColor[(location.x)+x][(location.y)+y]=pColor[x][y];
				}
			}
		}

		for(int y=0;y<PIECE_HEIGHT;y++){//右側移動
			for(int x=0;x<PIECE_WIDTH;x++){    
				piece[x][y]=tmp2[x][y];
				pColor[x][y]=tmp2Col[x][y];
			}
		}
		while(MovePiece(PIECE_RIGHT)==TRUE){
			Sleep(1);
		}

		for(int y=0;y<PIECE_HEIGHT;y++){
			for(int x=0;x<PIECE_WIDTH;x++){    
				if(piece[x][y] && (location.y)+y>=0){
					field[(location.x)+x][(location.y)+y]=piece[x][y];
					fColor[(location.x)+x][(location.y)+y]=pColor[x][y];
				}
			}
		}
	}else{
		for(int y=0;y<PIECE_HEIGHT;y++){
			for(int x=0;x<PIECE_WIDTH;x++){    
				if(piece[x][y] && (location.y)+y>=0){
					field[(location.x)+x][(location.y)+y]=piece[x][y];
					fColor[(location.x)+x][(location.y)+y]=pColor[x][y];
				}
			}
		}
	}
}

void DrawSystem(int score,int turn){
	SetFontSize((CELL_WIDTH+CELL_HEIGHT)/2);
	DrawFormatString((FIELD_WIDTH+2)*CELL_WIDTH,2,0xFFFFFFFF,"score:%d",score);
	DrawFormatString((FIELD_WIDTH+2)*CELL_WIDTH,2+CELL_HEIGHT,0xFFFFFFFF,"turn :%d",turn);
	int difx = (FIELD_WIDTH+2)*CELL_WIDTH,dify=(FIELD_HEIGHT)*CELL_HEIGHT;
	switch(difficult){
		case 0:{
			DrawString(difx,dify,"Normal",0xFFFFFFFF);
			break;}
		case 1:{
			DrawString(difx,dify,"Hard",0xFFFFFFFF);
			break;
			   }
		case 2:{
			DrawString(difx,dify,"VeryHard",0xFFFFFFFF);
			break;
			   }
		case 3:{
			DrawString(difx,dify,"UltraHard",0xFFFFFFFF);
			break;
			   }
		case 4:{
			DrawString(difx,dify,"Chaos",0xFFFFFFFF);
			break;
			   }
		case 5:{
			DrawString(difx,dify,"VeryChaos",0xFFFFFFFF);
			break;
			   }
		case 6:{
			DrawString(difx,dify,"UltraChaos",0xFFFFFFFF);
			break;
			   }
		 case 7:{
			DrawString(difx,dify,"Crazy",0xFFFFFFFF);
			break;
			   }
		case 8:{
			DrawString(difx,dify,"VeryCrazy",0xFFFFFFFF);
			break;
			   }
		case 9:{
			DrawString(difx,dify,"Unbelievable",0xFFFFFFFF);
			break;
			   }
		default:{
			DrawString(difx,dify,"　 ??? 　",0xFFFFFFFF);
			break;
			   }
		}
}

void MovePieceGimmick(){
	int x1=0,y1=0,x2=0,y2=0,find=0;
	int hozix[FIELD_HEIGHT*FIELD_WIDTH] = {0};
	int hoziy[FIELD_HEIGHT*FIELD_WIDTH] = {0};
	int hoziCol[FIELD_HEIGHT*FIELD_WIDTH] = {0};

	for(y1=0;y1<FIELD_HEIGHT;y1++){    // ゲームフィールドのブロック
        for(x1=0;x1<FIELD_WIDTH;x1++){
			if(field[x1][y1]==1&&field[x1+1][y1]==1&&field[x1][y1+1]==1
				&&field[x1-1][y1]==1&&field[x1][y1-1]==1){

					hozix[find] = x1;
					hoziy[find] = y1;
					hoziCol[find] = fColor[x1][y1];
					find++;

			}
        }
    }
	if(find==0)return;

	srand((unsigned) time(NULL));
	int randpiece = rand()%find;

	field[hozix[randpiece]][hoziy[randpiece]] = 0;

	while(true){
		int randx = rand()%FIELD_WIDTH;
		int randy = rand()%(FIELD_HEIGHT-5)+5;
		if(field[randx][randy]==0){

			int sax = (hozix[randpiece] - randx)*-1;
			int say = (hoziy[randpiece] - randy)*-1;

			SetDrawScreen( DX_SCREEN_BACK ) ;
			for(int i=0;i<10;i++){
				ClearDrawScreen();
				paint();
				drawBlock( (hozix[randpiece]+2)*CELL_WIDTH+((sax*CELL_WIDTH)/10*i),
					(hoziy[randpiece])*CELL_HEIGHT+((say*CELL_HEIGHT)/10*i),hoziCol[randpiece]);
				//fColor[hozix[randpiece]+(sax/10*i)][hoziy[randpiece]+(say/10*i)] = hoziCol[randpiece];
				
				ScreenFlip();
				Sleep(100);
			}
			SetDrawScreen( DX_SCREEN_FRONT ) ;

			field[randx][randy]=1;
			fColor[randx][randy] = hoziCol[randpiece];
			break;
		}

	}
	/*for(y2=0;y2<FIELD_HEIGHT;y2++){    // ゲームフィールドのブロック
		for(x2=0;x2<FIELD_WIDTH;x2++){
			if(field[x2][y2]==0){
				field[x2][y2]=1;
				return;
			}
		}
	}*/
	
}

void InitField(){
	for(int x=0;x<FIELD_WIDTH;x++){
		for(int y=0;y<FIELD_HEIGHT;y++){
			field[x][y] = 0;
		}
	}
}

// プログラムは WinMain から始まります
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
						LPSTR lpCmdLine, int nCmdShow )
{
	ChangeWindowMode( TRUE ) ;
	if( DxLib_Init() == -1 )		// ＤＸライブラリ初期化処理
	{
		return -1 ;			// エラーが起きたら直ちに終了
	}

	start:
	// 描画先画面を裏画面にセット
	//SetDrawScreen( DX_SCREEN_BACK ) ;

	//DrawBox(location.x,location.y,location.x+10,location.y+10,GetColor( 255,255,255 ), TRUE ) ; 
	
	int line=0,score=0,x,y,i=0,turn=0;
	
	bool downed = false;
	bool downed2 = false;
	bool downed3 = false;
	bool titleDown = false;
	int difx=230,dify=250;
	while( ProcessMessage() == 0 && CheckHitKey( KEY_INPUT_ESCAPE ) == 0 ){
		
		static int count;
		count++;
		ClearDrawScreen() ;
		SetFontSize(100);
		ChangeFont( "ＭＳ 明朝" ) ;
		DrawString(120,80,"TETRIS D",0xFFFFFFFF);
		SetFontSize(20);

		if(CheckHitKey( KEY_INPUT_RIGHT)&&difficult<9&&titleDown==false)difficult++;
		if(CheckHitKey( KEY_INPUT_LEFT)&&difficult>0&&titleDown==false)difficult--;

		if(titleDown==false&&(CheckHitKey( KEY_INPUT_RIGHT)||CheckHitKey( KEY_INPUT_LEFT)))titleDown=true;
		else if(!(CheckHitKey( KEY_INPUT_RIGHT))&&!(CheckHitKey( KEY_INPUT_LEFT))) titleDown=false;

		switch(difficult){
			
		case 0:{
			DrawString(difx,dify,"　  Normal   →",0xFFFFFFFF);
			break;}
		case 1:{
			DrawString(difx,dify,"←   Hard    →",0xFFFFFFFF);
			break;
			   }
		case 2:{
			DrawString(difx,dify,"← VeryHard  →",0xFFFFFFFF);
			break;
			   }
		case 3:{
			DrawString(difx,dify,"← UltraHard →",0xFFFFFFFF);
			break;
			   }
		case 4:{
			DrawString(difx,dify,"←   Chaos   →",0xFFFFFFFF);
			break;
			   }
		case 5:{
			DrawString(difx,dify,"← VeryChaos →",0xFFFFFFFF);
			break;
			   }
		case 6:{
			DrawString(difx,dify,"←UltraChaos →",0xFFFFFFFF);
			break;
			   }
		case 7:{
			DrawString(difx,dify,"←   Crazy   →",0xFFFFFFFF);
			break;
			   }
		case 8:{
			DrawString(difx,dify,"← VeryCrazy →",0xFFFFFFFF);
			break;
			   }
		case 9:{
			DrawString(difx,dify,"←Unbelievable",0xFFFFFFFF);
			break;
			   }
		default:{
			DrawString(difx,dify,"　 ??? 　",0xFFFFFFFF);
			break;
			   }
		}
		
		if(count%15>=3)DrawString(difx-70,dify+60,"PRESS SPACE TO START THE GAME",0xFFFFFFFF);

		if(CheckHitKey(KEY_INPUT_SPACE))break;
		ScreenFlip();
		
		
		Sleep(100);
		

	}

	InitField();
	NextPiece(true);

	if(difficult==3){
		srand((unsigned) time(NULL));
		for(int y=5;y<FIELD_HEIGHT;y++){
			for(int x=0;x<FIELD_WIDTH;x++){ 
				if(rand()%(FIELD_WIDTH/2)==0){
					field[x][y] = 1;
					fColor[x][y] = (rand()*rand())%0xffffffff;
				}
			}
		 }
	}
	if(difficult==2){
		srand((unsigned) time(NULL));
		for(int y=5;y<FIELD_HEIGHT;y++){
			for(int x=0;x<FIELD_WIDTH;x++){ 
				if(rand()%(FIELD_WIDTH*3)==0){
					field[x][y] = 1;
					fColor[x][y] = (rand()*rand())%0xffffffff;
				}
			}
		 }
	}
	while( ProcessMessage() == 0 && CheckHitKey( KEY_INPUT_ESCAPE ) == 0 ){

		if(i%10==0){
			if(CheckHitKey( KEY_INPUT_RIGHT ))MovePiece(PIECE_RIGHT);
			else if(CheckHitKey( KEY_INPUT_LEFT ))MovePiece(PIECE_LEFT);
			else if(CheckHitKey( KEY_INPUT_DOWN ))MovePiece(PIECE_DOWN);
			else if(CheckHitKey( KEY_INPUT_UP )){
				for(int i=0;i<FIELD_HEIGHT;i++){
					MovePiece(PIECE_DOWN);
				}
			}
			
		}

		if(CheckHitKey( KEY_INPUT_SPACE )&&downed==false){
			TurnPiece();
			downed = true;
		}
		if(CheckHitKey( KEY_INPUT_Z )&&downed2==false){
			HoldBlock(0);
			downed2=true;
		}
		if(CheckHitKey( KEY_INPUT_X )&&downed3==false){
			HoldBlock(1);
			downed3=true;
		}

		if(CheckHitKey(KEY_INPUT_F5))goto start;

		/*if(CheckHitKey( KEY_INPUT_C )){
			MovePieceGimmick();
		}*/

		if(!CheckHitKey( KEY_INPUT_SPACE )&&downed==true)downed=false;
		if(!CheckHitKey( KEY_INPUT_Z )&&downed2==true)downed2=false;
		if(!CheckHitKey( KEY_INPUT_X )&&downed3==true)downed3=false;

		int speed=0;
		switch(difficult){
		case 0:
			if(turn/2>60)speed=60;
			else speed=turn/2;
			break;
		default:
			if(turn+35>75)speed=75;
			else speed=turn+35;
		}
		
		if(i%(80-speed)==0){
			//MovePiece(PIECE_DOWN);
			if(!MovePiece(PIECE_DOWN)){    // 現在移動中のブロックが下段に達したら↓
				PieceToField();
				line=DeleteLine();
				turn++;
				if(line>0){
					for(y=0;y<PIECE_HEIGHT;y++){
						for(x=0;x<PIECE_WIDTH;x++){
							piece[x][y]=0;
						}
					}
					if(line==4){
						score+=1000;
						if(turn<40)turn = 0;
						else turn-=40;
					}
					else{
						if(turn<10)turn = 0;
						else turn-=10;
						score+=(line*100);
					}
					Sleep(500);
					if(difficult!=9)ShiftLine(line);
					
				}
				if(field[6][0] || field[7][0]){
					ClearDrawScreen();
					ScreenFlip();
					Sleep(1000);
					goto start;
				}
				Sleep(speed*2);
				srand((unsigned) time(NULL));
				if( (difficult>=1&&difficult<=3))
					MovePieceGimmick();
				
				NextPiece(FALSE);
			}
		}

		
		// 画面を初期化する
		ClearDrawScreen() ;
		paint();
		DrawSystem(score,turn);
		//ScreenFlip();
		
		Sleep(10);
		i++;


		
	}
	WaitKey() ;				// キー入力待ち

	DxLib_End() ;				// ＤＸライブラリ使用の終了処理

	return 0 ;				// ソフトの終了 
}

