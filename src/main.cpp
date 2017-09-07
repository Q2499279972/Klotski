#include <stdio.h>
#include <windows.h>
#include "window.hpp"
#include "Klotski.hpp"
#include "texture.hpp"
#include "misc.hpp"
#include "history.hpp"
#include "RightButtonMenu.hpp"
#include "ReadLuaConfig.hpp"
#include <gl/glext.h>

#define VERSIONSTRING  "0.01"

static int delaycnt=0;

void KlotskiBlock::Draw()
{
    int j;
    if(Id=='-')
    {
        return;
    }
    else if(Id=='#')
    {
        for(j=0;j<Width*Height;j++)
        {
            if(Shape[j]==' ') continue;
            int x=j%Width+Posx;
            int y=j/Width+Posy;
            glBegin(GL_QUADS);
                glColor4f(0,0,0,0);
                glVertex2f(x,y);
                glVertex2f(x,y+1);
                glVertex2f(x+1,y+1);
                glVertex2f(x+1,y);
            glEnd();
        }
        return;
    }
    else if(Id=='.')
    {
        for(j=0;j<Width*Height;j++)
        {
            if(Shape[j]==' ') continue;
            int x=j%Width+Posx;
            int y=j/Width+Posy;
            unsigned char picnum=ImageMappingMask[j];
            int a=picnum%16;
            int b=picnum/16;
            glBegin(GL_QUADS);
                glColor3f(1,1,1);
                glTexCoord2f(a/16.0, b/16.0/4+1/2.0);
                glVertex2f(x,y);
                glTexCoord2f(a/16.0, (b+1)/16.0/4+1/2.0);
                glVertex2f(x,y+1);
                glTexCoord2f((a+1)/16.0, (b+1)/16.0/4+1/2.0);
                glVertex2f(x+1,y+1);
                glTexCoord2f((a+1)/16.0, b/16.0/4+1/2.0);
                glVertex2f(x+1,y);
            glEnd();
        }
        return;
    }
    else
    {
        float k;
        if(Id=='*'||Id=='&') 
            k=1.0f;
        else
            k=0.0f;
        for(j=0;j<Width*Height;j++)
        {
            if(Shape[j]==' ') continue;
            int x=j%Width+Posx;
            int y=j/Width+Posy;
            unsigned char picnum=ImageMappingMask[j];
            int a=picnum%16;
            int b=picnum/16;
        
            glBegin(GL_QUADS);
                glColor3f(1,1,1);
                glTexCoord2f(a/16.0, b/16.0/4+k/4);
                glVertex2f(x,y);
                glTexCoord2f(a/16.0, (b+1)/16.0/4+k/4);
                glVertex2f(x,y+1);
                glTexCoord2f((a+1)/16.0, (b+1)/16.0/4+k/4);
                glVertex2f(x+1,y+1);
                glTexCoord2f((a+1)/16.0, b/16.0/4+k/4);
                glVertex2f(x+1,y);
            glEnd();
        }
    }
}

MSG         msg;
int         GameExit=0;

int state;
Klotski *gKlotski;
Window  *gKlotskiWindow;

POINT pt;
int id;
char LuaFilePath[256];

static LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    switch (uMsg)
    {
        case WM_DROPFILES:
        {
            UINT count;
            HDROP hDropInfo = (HDROP)wParam;
            count = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
            if( count==1 )
            {
                DragQueryFile(hDropInfo, 0, LuaFilePath, sizeof(LuaFilePath));
                DragFinish(hDropInfo);
                printf("Drag in file %s\n",LuaFilePath);
                gKlotski->setState(IDLE);
                state=5;
            }
            break;
        }
        case WM_LBUTTONDOWN:
        {
            int x=pt.x = GET_X_LPARAM(lParam);  
            int y=pt.y = GET_Y_LPARAM(lParam);  
            printf("LBUTTONDOWN %d %d\n",pt.x,pt.y);
            if(gKlotski->getState()!=NORMAL)
                break;
            x/=gKlotskiWindow->BlockPixel;
            y/=gKlotskiWindow->BlockPixel;
            id=gKlotski->Map->GetBlockId(x,y);
            printf("id= %d %c \n",id, id);
            break;
        }
        case WM_LBUTTONUP:
        {
            POINT pt2;
            printf("LBUTTONUP %d %d\n",pt.x,pt.y);
            if(gKlotski->getState()!=NORMAL)
                break;
            int x=pt2.x = GET_X_LPARAM(lParam);  
            int y=pt2.y = GET_Y_LPARAM(lParam);  
            int dx=pt2.x-pt.x;
            int dy=pt2.y-pt.y;
            if(id==' ') return 0;
            if(abs(dx)>abs(dy))
            {
                if(dx>gKlotskiWindow->BlockPixel*2/3)
                {
                    gKlotski->DoMove(id,RIGHT);
                }
                if(dx<-gKlotskiWindow->BlockPixel*2/3)
                {
                    gKlotski->DoMove(id,LEFT);
                }
            }
            else
            {
                if(dy>gKlotskiWindow->BlockPixel*2/3)
                {
                    gKlotski->DoMove(id,DOWN);
                }
                if(dy<-gKlotskiWindow->BlockPixel*2/3)
                {
                    gKlotski->DoMove(id,UP);
                }
            }
            if(gKlotski->checkTargetSuccess()==0)
            {    
                char str[50];
                snprintf(str,50,"%s %d/%d",gKlotski->cfg.title.c_str(),gKlotski->His->CurrentStep,gKlotski->His->TotalStep);
                gKlotskiWindow->SetTitle(str);
                gKlotski->setState(NORMAL);
            }
            else
            {
                char str[50];
                snprintf(str,50,"%s successed %d/%d",gKlotski->cfg.title.c_str(),gKlotski->His->CurrentStep,gKlotski->His->TotalStep);
                gKlotskiWindow->SetTitle(str);
                gKlotski->setState(FINISHED);
            }
            break;
        }
        case WM_CLOSE:
        {
            PostQuitMessage(0);
            return( 0 );
            break;
        }
        case WM_KEYDOWN:
        {
            if(gKlotski->getState()!=NORMAL && gKlotski->getState()!=FINISHED)
                break;
            if(wParam=='B')
            {
                if( gKlotski->His->CanBackward())
                {
                    printf("UNDO\n");
                    StepInfo step = gKlotski->His->GetBackwardStep();
                    gKlotski->His->DoBackward();
                    if(step.move==LEFT) step.move=RIGHT;
                    else if(step.move==RIGHT) step.move=LEFT;
                    else if(step.move==UP) step.move=DOWN;
                    else if(step.move==DOWN) step.move=UP;
                    if(step.move==UP)
                        gKlotski->Map->SearchBlockById(step.id)->DoMoveUp();
                    if(step.move==DOWN)
                        gKlotski->Map->SearchBlockById(step.id)->DoMoveDown();
                    if(step.move==LEFT)
                        gKlotski->Map->SearchBlockById(step.id)->DoMoveLeft();
                    if(step.move==RIGHT)
                        gKlotski->Map->SearchBlockById(step.id)->DoMoveRight();            
                    gKlotski->Map->UpdateMap();
                    gKlotski->Map->Print();
                    if(gKlotski->checkTargetSuccess()==0)
                    {    
                        char str[50];
                        snprintf(str,50,"%s %d/%d",gKlotski->cfg.title.c_str(),gKlotski->His->CurrentStep,gKlotski->His->TotalStep);
                        gKlotskiWindow->SetTitle(str);
                        gKlotski->setState(NORMAL);
                    }
                    else
                    {
                        char str[50];
                        snprintf(str,50,"%s successed %d/%d",gKlotski->cfg.title.c_str(),gKlotski->His->CurrentStep,gKlotski->His->TotalStep);
                        gKlotskiWindow->SetTitle(str);
                        gKlotski->setState(FINISHED);
                    }
                }
            }
            if(wParam=='F')
            {
                if( gKlotski->His->CanForward())
                {
                    printf("REDO\n");
                    StepInfo step = gKlotski->His->GetForwardStep();
                    gKlotski->His->DoForward();
                    if(step.move==UP)
                        gKlotski->Map->SearchBlockById(step.id)->DoMoveUp();
                    if(step.move==DOWN)
                        gKlotski->Map->SearchBlockById(step.id)->DoMoveDown();
                    if(step.move==LEFT)
                        gKlotski->Map->SearchBlockById(step.id)->DoMoveLeft();
                    if(step.move==RIGHT)
                        gKlotski->Map->SearchBlockById(step.id)->DoMoveRight();            
                    gKlotski->Map->UpdateMap();
                    gKlotski->Map->Print();
                    if(gKlotski->checkTargetSuccess()==0)
                    {    
                        char str[50];
                        snprintf(str,50,"%s %d/%d",gKlotski->cfg.title.c_str(),gKlotski->His->CurrentStep,gKlotski->His->TotalStep);
                        gKlotskiWindow->SetTitle(str);
                        gKlotski->setState(NORMAL);
                    }
                    else
                    {
                        char str[50];
                        snprintf(str,50,"%s successed %d/%d",gKlotski->cfg.title.c_str(),gKlotski->His->CurrentStep,gKlotski->His->TotalStep);
                        gKlotskiWindow->SetTitle(str);
                        gKlotski->setState(FINISHED);
                    }
                }
            }
            break;
        }
        case WM_CONTEXTMENU:
        {
            POINT p = {LOWORD(lParam),HIWORD(lParam)};
            HMENU hMainMenu = GetMenu(hWnd);
            HMENU m = CreatePopupMenu();
            AppendMenu(m, MF_STRING, 0x8001,"Load level file");
            AppendMenu(m, MF_STRING, 0x8002,"Save screen");
            AppendMenu(m, MF_STRING, 0x8003,"Save steps");
            if(gKlotski->getState()!=IDLE && gKlotski->His->CurrentStep==0)
            {
                AppendMenu(m, MF_STRING, 0x8004,"Auto (file.txt)");
            }
            AppendMenu(hMainMenu, MF_STRING | MF_POPUP, (UINT)m, "main");
            TrackPopupMenu(m, TPM_VERTICAL|TPM_TOPALIGN, p.x, p.y, 0, hWnd, NULL);
            DestroyMenu(m);
            break;
        }
        case WM_COMMAND:
        {
            switch (wParam)
            {
                case 0x8001:
                {
                    OPENFILENAME ofn;       // common dialog box structure
                    char szFile[260];       // buffer for file name
                    // Initialize OPENFILENAME
                    ZeroMemory(&ofn, sizeof(ofn));
                    ofn.lStructSize = sizeof(ofn);
                    ofn.hwndOwner = 0;
                    ofn.lpstrFile = szFile;
                    // Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
                    // use the contents of szFile to initialize itself.
                    ofn.lpstrFile[0] = '\0';
                    ofn.nMaxFile = sizeof(szFile);
                    ofn.lpstrFilter = "All\0*.*\0Lua\0*.Lua\0";
                    ofn.nFilterIndex = 1;
                    ofn.lpstrFileTitle = NULL;
                    ofn.nMaxFileTitle = 0;
                    ofn.lpstrInitialDir = NULL;
                    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
                    // Display the Open dialog box. 
                    if (GetOpenFileName(&ofn)==TRUE)
                    {
                        strcpy(LuaFilePath,szFile);
                        gKlotski->setState(IDLE);
                        state=5;
                    }
                    break;
                }
                case 0x8002:
                {
                    //refered http://blog.csdn.net/dreamcs/article/details/6052984
                    static char head[54]=
                    {
                        0x42,0x4d,0x66,0x75,0x00,0x00,0x00,0x00,
                        0x00,0x00,0x36,0x00,0x00,0x00,0x28,0x00,
                        0x00,0x00,0x64,0x00,0x00,0x00,0x64,0x00,
                        0x00,0x00,0x01,0x00,0x18,0x00,0x00,0x00, 
                        0x00,0x00,0x30,0x75,
                    };
                    GLint pixelLength;  
                    GLubyte * pixelDate;  
                    FILE * wfile;
                    
                    int width=gKlotskiWindow->TotalPixel_x;
                    int height=gKlotskiWindow->TotalPixel_y;
                    pixelLength = width * 3; 
                    if ( pixelLength % 4 != 0 )  
                    {  
                        pixelLength += 4 - pixelLength%4;  
                    }  
                    pixelLength *= height;
                    pixelDate = (GLubyte *)malloc( pixelLength );
                    if ( pixelDate == 0 )  
                    {  
                        printf("error: malloc!\n");
                        break;
                    }
                    
                    char bmpName[100];
                    char str[50]={0};
                    if(gKlotski->getState()!=IDLE)
                        snprintf(str,50,"%s %d_%d",gKlotski->cfg.title.c_str(),gKlotski->His->CurrentStep,gKlotski->His->TotalStep);
                    SYSTEMTIME tm;
                    GetLocalTime(&tm);
                    snprintf(bmpName,100,".\\%s_%4.4d%2.2d%2.2d_%2.2dh%2.2dm%2.2ds.bmp",
                            str,
                            tm.wYear, 
                            tm.wMonth, 
                            tm.wDay,  
                            tm.wHour, 
                            tm.wMinute,
                            tm.wSecond);
                    printf("save screen to file %s\n",bmpName);        
                    wfile = fopen( bmpName, "wb" ); 
                    if(wfile==NULL)
                    {
                        free( pixelDate ); 
                        printf("can't open file %s\n",bmpName);
                        break;
                    }
                    fwrite( head, 54, 1, wfile );
                    fseek( wfile, 0x0012, SEEK_SET );
                    fwrite( &width, sizeof(width), 1, wfile );  
                    fwrite( &height, sizeof(height ), 1, wfile ); 

                    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);  
                    glReadPixels( 0, 0, width, height, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixelDate );
                    fseek( wfile, 0, SEEK_END );  
                    fwrite( pixelDate, pixelLength, 1, wfile ); 
                    
                    fclose( wfile );  
                    free( pixelDate ); 
                    break;
                }
                case 0x8003:
                {
                    if(gKlotski->getState()!=IDLE)
                    {
                        char str[50]={0};
                        char name[100];
                        FILE * wfile;
                        snprintf(str,50,"his_%s",gKlotski->cfg.title.c_str());
                        SYSTEMTIME tm;
                        GetLocalTime(&tm);
                        snprintf(name,100,".\\%s_%4.4d%2.2d%2.2d_%2.2dh%2.2dm%2.2ds.txt",
                                str,
                                tm.wYear, 
                                tm.wMonth, 
                                tm.wDay,  
                                tm.wHour, 
                                tm.wMinute,
                                tm.wSecond);
                        wfile = fopen( name, "w" );
                        if(wfile==NULL) break;
                        gKlotski->His->SaveHistory(wfile);
                        fclose(wfile);
                    }
                    break;
                }
                case 0x8004:
                {
                    OPENFILENAME ofn;       // common dialog box structure
                    char szFile[260];       // buffer for file name
                    // Initialize OPENFILENAME
                    ZeroMemory(&ofn, sizeof(ofn));
                    ofn.lStructSize = sizeof(ofn);
                    ofn.hwndOwner = 0;
                    ofn.lpstrFile = szFile;
                    // Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
                    // use the contents of szFile to initialize itself.
                    ofn.lpstrFile[0] = '\0';
                    ofn.nMaxFile = sizeof(szFile);
                    ofn.lpstrFilter = "All\0*.*\0txt\0*.txt\0";
                    ofn.nFilterIndex = 1;
                    ofn.lpstrFileTitle = NULL;
                    ofn.nMaxFileTitle = 0;
                    ofn.lpstrInitialDir = NULL;
                    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
                    // Display the Open dialog box. 
                    if (GetOpenFileName(&ofn)==TRUE)
                    {
                        FILE * file;
                        gKlotski->setState(AUTO);
                        file = fopen(szFile,"r");
                        if(file==NULL)
                            break;
                        if(!gKlotski->His->LoadHistory(file))
                        {
                            fclose(file);
                            break;
                        }
                        delaycnt=0;
                        printf("his load ok %d %d\n",gKlotski->His->CurrentStep,gKlotski->His->TotalStep);
                    }
                    break;
                }
            }
            break;
        }
        case WM_HELP:
        {
            ::MessageBoxA(0,"leave blank now ...","help",MB_HELP);
        }
    }
    return( DefWindowProc(hWnd,uMsg,wParam,lParam) );
}


int main( )
{
    //for debug
    CreateConsole();
    
    gKlotskiWindow=new Window(1,1,WndProc);
    gKlotskiWindow->Create();
    gKlotskiWindow->SetTitle("welcome");
    
    gKlotski = new Klotski();
    
    while(!GameExit)
    {
        switch (state)
        {
            case 0:// IDLE
            {
                break;
            }
            case 1:// INIT
            {
                if( gKlotski->Init(LuaFilePath))
                    state=2;
                else
                    state=0;
                break;
            }
            case 4://deinit
            {
                gKlotski->Deinit();
                state=0;
                break;
            }
            case 5://reinit
            {
                gKlotski->Deinit();
                if( gKlotski->Init(LuaFilePath))
                    state=2;
                else
                    state=0;
                break;
            }
            case 2://Prepare to work
            {
                
                gKlotskiWindow->Destroy();
                delete gKlotskiWindow;
                
                gKlotskiWindow=new Window(gKlotski->width,gKlotski->height,WndProc);
                gKlotski->Map->UpdateMap();
                gKlotski->Map->Print();
                gKlotskiWindow->Create();
                SetWindowTextA(gKlotskiWindow->hWnd, "111");
                
                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                gluOrtho2D(0,gKlotski->width,gKlotski->height,0);
                glEnable(GL_TEXTURE_2D);
                glEnable(GL_ALPHA_TEST); 
                glAlphaFunc(GL_LESS ,0.9);
                
                picgen();//see texture.cpp
                Texture texture1(32*16,32*16*4,GL_RGBA,(unsigned char *)buf2,0);
                picrelease();
                texture1.Bind();
                
                char str[50];
                snprintf(str,50,"%s %d/%d",gKlotski->cfg.title.c_str(),gKlotski->His->CurrentStep,gKlotski->His->TotalStep);
                gKlotskiWindow->SetTitle(str);
                gKlotski->setState(NORMAL);
                state=3;
                break;
            }
            case 3://work loop
            {
                glClearColor( 0.2f, 0.4f, 1.0f, 0.0f );
                glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );
                int i;
                for(i=0;i<gKlotski->Map->BlockNumber;i++)
                {
                    gKlotski->Map->Blocks[i].Draw();
                }
                break;
            }
        }
        
        SwapBuffers( gKlotskiWindow->hDC );
    
        if(gKlotski->getState()==AUTO)
        {
            delaycnt++;
            if(delaycnt==5)
            {
                delaycnt=0;
            }
            #ifdef _EXPORTBMP_
            if(delaycnt==3)
            {
                static char head[54]=
                {
                    0x42,0x4d,0x66,0x75,0x00,0x00,0x00,0x00,
                    0x00,0x00,0x36,0x00,0x00,0x00,0x28,0x00,
                    0x00,0x00,0x64,0x00,0x00,0x00,0x64,0x00,
                    0x00,0x00,0x01,0x00,0x18,0x00,0x00,0x00, 
                    0x00,0x00,0x30,0x75,
                };
                GLint pixelLength;  
                GLubyte * pixelDate;  
                FILE * wfile;
                
                int width=gKlotskiWindow->TotalPixel_x;
                int height=gKlotskiWindow->TotalPixel_y;
                pixelLength = width * 3; 
                if ( pixelLength % 4 != 0 )  
                {  
                    pixelLength += 4 - pixelLength%4;  
                }  
                pixelLength *= height;
                pixelDate = (GLubyte *)malloc( pixelLength );
                if ( pixelDate == 0 )  
                {  
                    printf("error: malloc!\n");
                }
                else
                {
                    char bmpName[100];
                    char str[50]={0};
                    if(gKlotski->getState()!=IDLE)
                        snprintf(str,50,"%s %d_%d",gKlotski->cfg.title.c_str(),gKlotski->His->CurrentStep,gKlotski->His->TotalStep);
                    SYSTEMTIME tm;
                    GetLocalTime(&tm);
                    snprintf(bmpName,100,".\\%s_%4.4d%2.2d%2.2d_%2.2dh%2.2dm%2.2ds.bmp",
                            str,
                            tm.wYear, 
                            tm.wMonth, 
                            tm.wDay,  
                            tm.wHour, 
                            tm.wMinute,
                            tm.wSecond);
                    printf("save screen to file %s\n",bmpName);        
                    wfile = fopen( bmpName, "wb" ); 
                    if(wfile==NULL)
                    {
                        free( pixelDate ); 
                        printf("can't open file %s\n",bmpName);
                    }
                    else
                    {
                        fwrite( head, 54, 1, wfile );
                        fseek( wfile, 0x0012, SEEK_SET );
                        fwrite( &width, sizeof(width), 1, wfile );  
                        fwrite( &height, sizeof(height ), 1, wfile ); 

                        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);  
                        glReadPixels( 0, 0, width, height, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixelDate );
                        fseek( wfile, 0, SEEK_END );  
                        fwrite( pixelDate, pixelLength, 1, wfile ); 
                        
                        fclose( wfile );  
                        free( pixelDate );
                    }
                }                    
            }
            #endif
            if(delaycnt==1)
            {
                printf("auto..\n");
                if( gKlotski->His->CanForward())
                {
                    printf("REDO\n");
                    StepInfo step = gKlotski->His->GetForwardStep();
                    gKlotski->His->DoForward();
                    if(step.move==UP)
                        gKlotski->Map->SearchBlockById(step.id)->DoMoveUp();
                    if(step.move==DOWN)
                        gKlotski->Map->SearchBlockById(step.id)->DoMoveDown();
                    if(step.move==LEFT)
                        gKlotski->Map->SearchBlockById(step.id)->DoMoveLeft();
                    if(step.move==RIGHT)
                        gKlotski->Map->SearchBlockById(step.id)->DoMoveRight();            
                    gKlotski->Map->UpdateMap();
                    gKlotski->Map->Print();
                    if(gKlotski->checkTargetSuccess()==0)
                    {    
                        char str[50];
                        snprintf(str,50,"%s %d/%d",gKlotski->cfg.title.c_str(),gKlotski->His->CurrentStep,gKlotski->His->TotalStep);
                        gKlotskiWindow->SetTitle(str);
                        gKlotski->setState(AUTO);
                    }
                    else
                    {
                        char str[50];
                        snprintf(str,50,"%s successed %d/%d",gKlotski->cfg.title.c_str(),gKlotski->His->CurrentStep,gKlotski->His->TotalStep);
                        gKlotskiWindow->SetTitle(str);
                        gKlotski->setState(AUTO);
                    }
                }
            }
            if(delaycnt==4)
            {
                if(gKlotski->checkTargetSuccess()==1)
                {
                    gKlotski->setState(FINISHED);
                }
                else 
                {
                    if(gKlotski->His->CanForward())
                    {
                        gKlotski->setState(AUTO);
                    }
                    else
                    {
                        gKlotski->setState(NORMAL);
                    }
                }
            }
        }
        while( PeekMessage(&msg,0,0,0,PM_REMOVE) )
        {
            if( msg.message==WM_QUIT ) 
            {
                GameExit=1;
            }
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
        Sleep(50);
    }
    gKlotskiWindow->Destroy();
    delete(gKlotskiWindow);
    delete(gKlotski);
    return 0;
}        

