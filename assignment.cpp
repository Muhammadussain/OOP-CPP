#include <iostream>
#include <string>
#include <utility>
using namespace std;

class Resources {
public:
   // virtual void load() const = 0;

    Resources(const std::string& n) : name(n) {}

    virtual ~Resources() {}

    Resources(const Resources&) = default;
    Resources(Resources&&) = default;
    
   virtual Resources& operator=(const Resources& rhs) {
        if(this==&rhs ){
            cout <<"self assignemnt "<<endl;
        return *this ;
        }
        name =rhs.name;
        return *this;
    };
    virtual Resources& operator=(Resources&& source){
        if(this==&source)
        {return *this ;
        }
        name=move(source.name);
        source.name="";
        return *this ;
    };


protected:
    string name;
};
class TextureResource:public Resources{
    public :
    int width ;
    int height ;
unsigned char* data;
};
int main() {
    Resources r1{"hussain"};
    Resources r2;
    r2=r1;
    return 0;
}