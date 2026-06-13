#include <iostream>
#include <cstdint>

class CallStack
{
    private:
        uint16_t data[16] = {0};
        uint8_t top = 0;
    public:
        void push(uint16_t address)
        {
            if (top < 16)
            {
                data[top] = address;
                top++;
            }
            else
            {
                // to any code reader that isn't me, this is a joke.
                std::cout << "Please check Stack Overflow to fix this error.\n You've made too many function calls in your script.\n";

                for (uint8_t i = 0; i < 15; i++)
                {
                    data[i] = data[i + 1];
                }
                top = 15;
                data[top] = address;
                // ^ shifts the addresses due to the stack overflowing
            }
        }

        uint16_t pop()
        {
            if (top > 0)
            {
                top--;
                return data[top];
            }
            // return 0 cuz the stack is empty
            return 0;
        }
};

void fetch();
void decode();
void execute();

int main()
{
    // Initialize the Components
    uint16_t instruction_memory[1024];
    uint16_t pc = 0;
    CallStack call_stack;
    uint8_t RAM[256];
    uint8_t register_file[8];
    bool zero_flag;
    bool carry_flag;

    // Load in da recipe (load in the program)
    
    return 0;
}