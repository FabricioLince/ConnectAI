#ifndef WINDOWHOLDER_H
#define WINDOWHOLDER_H
#include "LVector.h"
#include <SDL.h>
#include "Renderer.h"

/*** WindowHolder
    Classe responsável por Gerenciar o SDL_Window
    e o SDL_Renderer da aplicação, assim como suas
    propriedades (resolução, flags de criação, etc)
***/
class WindowHolder
{
    public: // temporário
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
            Chamando qualquer uma das funções a seguir para Window
            chama automaticamente a função equivalente para Renderer
            Mas chamar a função para Renderer NÃO chama a função
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
