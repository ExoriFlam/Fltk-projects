#include <iostream>
#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Output.H>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>




void updOutput(Fl_Output* out, std::string val)
{
    if (out)
    {
        out->value(val.c_str());
    }
}

double calculate(std::string left, std::string right, char operation)
{
    double result = 0;
    double lhs = std::stod(left);
    double rhs = std::stod(right);
    
    switch (operation)
    {
        case '+':
            result = lhs + rhs;
            break;
        case '-':
            result = lhs - rhs;
            break;
        case '*':
            result = lhs * rhs;
            break;
        case '/':
            result = (lhs == 0 || rhs == 0) ? 0 : lhs / rhs;
            break;
        case '%':
            break;
       
        default:
            break;
    }

    return result;
}

bool is_operator(char val)
{
    return (val == '+' || val == '-' || val == '*' || val == '/' || val == '%');
}

std::string format_result(double res)
{   
    std::string result = std::to_string(res);

    size_t pos = result.find_last_not_of('0');

    if (pos != std::string::npos)
    {
        result.erase(pos + 1);
    }

    if (!result.empty() && result.back() == '.')
    {
        result.pop_back();
    }

    return result;
}

void ButtonCb(Fl_Widget* widget, void* data)
{
    Fl_Output* out = (Fl_Output*)data;
    std::string val = out->value();

    std::string btn_name = widget->label();

    static bool nbr_selection = false;
    static bool operation = false;
    static std::string prev_val;
    static char oper;

    if (isdigit(btn_name[0]))
    {
        nbr_selection = true;

        if (val[0] == '0' || operation)
        {
            val = btn_name[0]; 
            operation = false;
        }
        else
        {
            val += btn_name[0];
        }
    }
    else if (is_operator(btn_name[0]))
    {
        operation = true;

        if (nbr_selection)
        {
            if (!prev_val.empty())
            {
                double result = calculate(prev_val, val, oper);
                val = format_result(result);

            }

            prev_val = val;

            nbr_selection = false;
        }
        
        oper = btn_name[0];
    }

    updOutput(out, val.substr(0, 15));
   
}





int main()
{

    Fl_Window* window = new Fl_Window(400, 600, "Calculator");
  
    window->begin();

    Fl_Output* output = new Fl_Output(2,2,395,95);
    Fl_Box* output_box = new Fl_Box(0, 0, 400, 100);
    output->color(FL_GRAY);
    output->textsize(45);
    
    output->value("0");
    
    
    /*output->maximum_size(12);
    output->align(FL_ALIGN_LEFT);*/
    std::vector<std::string> btn_lables
    {
        "%","C","CE","<=","7","8","9","*","4","5","6","-","1","2","3","+","/","0",".","="
    };

    std::vector<std::vector<Fl_Box*>> boxes;
    std::vector<std::vector<Fl_Button*>> buttons;
    boxes.resize(5, std::vector<Fl_Box*>(4));
    buttons.resize(5, std::vector<Fl_Button*>(4));
    
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 4; ++j) {
            boxes[i][j] = new Fl_Box(100 * j, 100 * i + 100, 100, 100, "");
            boxes[i][j]->box(FL_DOWN_FRAME);
            
            
        }
    }

    int name = 0;

    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 4; ++j) {
           
            buttons[i][j] = new Fl_Button(100 * j + 2, 100 * i + 102, 95, 95, "");
            buttons[i][j]->labelsize(50);
            buttons[i][j]->copy_label(btn_lables[name++].c_str());
            buttons[i][j]->callback(ButtonCb, output);
           
        }
    }
    
    window->end();

   
    window->show();

    
    return Fl::run();
}