
#include <Editor/VisualScripting/GenTypes.h>

#include <vector>

#include <string>


using namespace std;

class Gen_Component_Init
{
public:
   static void Init() {
      GenTypes::RegisterType("NewComponent*");
      GenTypes::RegisterType("NewComponent1*");
      GenTypes::RegisterType("NewComponent11*");
   };
};

struct NewComponent;
struct NewComponent1;
struct NewComponent11;

//0
struct NewComponent{
   std::vector<bool> newVariable2;
   std::vector<NewComponent11*> newVariable;
   std::vector<bool> newVariable1;
   std::vector<bool> newVariable3;
};
//1
struct NewComponent1{
   std::vector<bool> newVariable;
};
//2
struct NewComponent11{
   NewComponent* newVariable;
};
