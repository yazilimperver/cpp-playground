#include "sdl-application.h"

int main(int argc, char* argv[]) {
    Sdl3Application application;
    
    if (!application.Initialize()) {
        return -1;
    }
    
    std::cout << "Cikis icin Q tusuna basiniz!\n";
    std::cout << "WASD tuslari ile yesil dikdortgen hareket ettirilebilir!\n";
    std::cout << "Bu tuslar disindaki tuslar hareketi sonlandirir!\n";
    
    application.Run();
    application.Shutdown();
    
    return 0;
}