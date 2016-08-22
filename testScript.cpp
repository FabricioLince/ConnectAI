#include <dirent.h>
#include <iostream>

int main()
{
    const char* path = "imgs/Connect_Tests";
    std::cout << "Contents of " << path << "\n";
    FILE* pipe = NULL;
    DIR* directory = opendir(path);
    if (directory)
    {
        struct dirent* content = readdir(directory);
        for (; content; content = readdir(directory))
        {
            if (strstr(content->d_name, ".png") != NULL)
            {
                char cmd[128];
                sprintf(cmd, "LL_AI.exe \"%s/%s\" 1", path, content->d_name);
                std::cout << content->d_name << "\n";
                pipe = popen(cmd, "r");
                int rt = pclose(pipe);
                if (rt == 0)
                {
                    std::cout << "\tResolvido!\n";
                }
                else
                {
                    std::cout << "\t\tErro...\n";
                }
            }
        }
        closedir(directory);
        system("pause");
    }
    else
    {
        std::cout << "couldn't open dir\n";
    }
    return 0;
}
