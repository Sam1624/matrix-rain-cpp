#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <thread>
#include <chrono>
#include <csignal>
#include <sys/ioctl.h>
#include <unistd.h>

struct TerminalSize { int width; int height; };
volatile bool keep_running = true;

void signal_handler(int signum) { keep_running = false; }

TerminalSize get_terminal_size() {
    winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return {w.ws_col, w.ws_row};
}

void move_cursor(int x, int y) {
    std::cout << "\033[" << y + 1 << ";" << x + 1 << "H";
}

struct Drop {
    int x;
    float y;
    float speed;
    int tail_length;
};

int main() {
    signal(SIGINT, signal_handler);

    TerminalSize term_size = get_terminal_size();
    int width = term_size.width;
    int height = term_size.height;

    std::vector<std::string> screen_buffer(height, std::string(width, ' '));
    std::vector<std::string> prev_buffer(height, std::string(width, ' '));

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> char_dist(33, 126);
    std::uniform_int_distribution<int> len_dist(height / 3, height - 2);
    std::uniform_real_distribution<float> speed_dist(0.5f, 1.5f);

    std::vector<Drop> drops;
    for (int i = 0; i < width; ++i) {
        if (gen() % 4 != 0) continue; 
        drops.push_back({
            i,
            (float)(-len_dist(gen)),
            speed_dist(gen),
            len_dist(gen)
        });
    }

    std::cout << "\033[?25l" << "\033[2J" << std::flush;

    while (keep_running) {
        for(int y=0; y<height; ++y) {
            screen_buffer[y] = std::string(width, ' ');
        }
        
        for (auto& drop : drops) {
            for (int i = 0; i < drop.tail_length; ++i) {
                int current_y = static_cast<int>(drop.y) - i;
                
                if (current_y >= 0 && current_y < height) {
                    char symbol;
                    if (i == 0) symbol = 'W'; 
                    else if (i < 3) symbol = 'G';
                    else symbol = 'D';
                    
                    screen_buffer[current_y][drop.x] = symbol;
                }
            }

            drop.y += drop.speed;

            if (drop.y - drop.tail_length > height) {
                drop.y = 0;
                drop.tail_length = len_dist(gen);
                drop.speed = speed_dist(gen);
            }
        }

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                if (screen_buffer[y][x] != prev_buffer[y][x]) {
                    move_cursor(x, y);
                    char c = screen_buffer[y][x];
                    switch(c) {
                        case 'W': std::cout << "\033[1;37m" << (char)char_dist(gen); break; 
                        case 'G': std::cout << "\033[1;32m" << (char)char_dist(gen); break; 
                        case 'D': std::cout << "\033[0;32m" << (char)char_dist(gen); break; 
                        default:  std::cout << ' '; break;
                    }
                }
            }
        }
        std::cout.flush();

        prev_buffer = screen_buffer;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    std::cout << "\033[?25h" << "\033[0m" << "\033[2J" << "\033[H" << std::flush;
    return 0;
}
