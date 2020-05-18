#include <iostream>
#include <cmath>

double convert_string_to_param(const std::string& string)
{
  std::string first;
  double power_of_ten = 0;
  double mil = 1;
  bool done = false;
  bool meg_mil = false;


  for(int i = 0; i<string.size() && !done; i++) {
    if(isdigit(string[i]) || (string[i] == '.'))
    {
      first.push_back(string[i]);
    }
    else
    {
      done = true;
      switch (string[i])
      {

        case 'f':
        case 'F':

          power_of_ten = -15;
          break;

        case 'p': //pico
        case 'P':

          power_of_ten = -12;
          break;

        case 'n': //nano
        case 'N':

          power_of_ten = -9;
          break;

        case 'u': //micro, check if the character is right!!!
        case 'U':

          power_of_ten = -6;
          break;

        case 'm':
        case 'M':

            if(string.size()> i + 2) {
            if(((string[i+1]=='E' )|| (string[i+1]=='e') )&& ((string[i+2]=='G')||(string[i+2]=='g')))
            //prefix is MEG
            {
              meg_mil = true;
              power_of_ten = 6;
            }
            else  if(((string[i+1]=='I') || (string[i+1]=='i' ))&& ((string[i+2]=='L')||(string[i+2]=='l')))
            //prefix is MIL
            {
              power_of_ten = -6;
              mil = 25.4;
              meg_mil = true;

            }
          }
          if(!meg_mil) //if it wasn't mega nor mil, it's milli
          {
            power_of_ten = -3;
          }
          break;

        case 'k': //kilo
        case 'K':

          power_of_ten = 3;
          break;

        case 'g': //giga
        case 'G':

          power_of_ten=9;
          break;

        case 't': //terra
        case 'T':

          power_of_ten = 12;
          break;

        default:

          std::cerr << "Invalid prefix";
          break;

      }

}

      }

      return (strtod(first.c_str(), nullptr) * mil * pow(10, power_of_ten));
}

  /*    int main()
      {

        std::string str;
        std::cin>>str;
        std::cout<< convert_string_to_param(str);
        return 0;
      }
*/
