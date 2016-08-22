#ifndef WINDOWHOLDER_H
#define WINDOWHOLDER_H
#include "LVector.h"
#include <SDL.h>
#include "Renderer.h"

/*** WindowHolder
    Classe respons�vel por Gerenciar o SDL_Window
    e o SDL_Renderer da aplica��o, assim como suas
    propriedades (resolu��o, flags de cria��o, etc)
***/
class WindowHolder
{
    public: // tempor�rio
    I_Vector screenResolution;
    bool fullscreen;
    LWindow window;
    Renderer renderer;
    int displayNumber;

    public:
        WindowHolder();
        ~WindowHolder();
        SDL_Window* getWindow();

        /**
            Chamando qualquer uma das fun��es a seguir para Window
            chama automaticamente a fun��o equivalente para Renderer
            Mas chamar a fun��o para Renderer N�O chama a fun��o
            equivalente para Window
        **/
        bool createWindow();
        bool createRenderer();
        void destroyWindow();
        void destroyRenderer();
        bool reloadWindow();
        bool reloadRenderer();
        bool setScreenResolution(int x, int y);
        bool isValidScreenResolution(int x, int y);
        bool setFullscreen(bool state);
        bool changeDisplay(int index);

        Renderer* getRenderer();
};
#endif // WINDOWHOLDER_H
