#include <iostream>
#include <string>
#include <utility>
using namespace std;

class Resource {
public:
   // virtual void load() const = 0;

    Resource(const std::string& n) : name(n) {}

    virtual ~Resource() {}

    Resource(const Resource&) = default;
    Resource(Resource&&) = default;
    
   virtual Resource& operator=(const Resource& rhs) {
        if(this==&rhs ){
            cout <<"self assignemnt "<<endl;
        return *this ;
        }
        name =rhs.name;
        return *this;
    };
    virtual Resource& operator=(Resource&& source){
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
class TextureResource:public Resource{
    public :
    int width ;
    int height ;
unsigned char* data;

TextureResource( string n,int w,int h):Resource(n),width(w),height(h){
        data=new unsigned char[width*height*4];
};
~TextureResource(){
    delete[] data;
};
TextureResource(const TextureResource& other):Resource(other),width(other.width),height(other.height),data(other.data){};
TextureResource(TextureResource&& other ):Resource(move(other)),width(other.width),height(other.height),data(other.data){
    other.data=nullptr;
};
TextureResource& operator=(const TextureResource& rhs) override{
    
    const TextureResource* ptr=dynamic_cast<const TextureResource*> (&rhs);

    if(!ptr){
        throw std::runtime_error("type mismatched");
    }



    if(this==&rhs) return *this;
    Resource::operator=(rhs);
    delete [] data;
    width=rhs.width;
    height=rhs.width;
    data=new unsigned char [width*height*4];
    for (int i=0;i<width*height*4;i++){
        data[i]=rhs.data[i];

    }
    return *this;
};
TextureResource& operator=(TextureResource&& rhs) override{
    TextureResource* ptr=dynamic_cast<TextureResource*>(&rhs);

    if(!ptr){
        throw std::runtime_error("type mis matched ");
    }

    if(this==&rhs) return *this ;
    Resource::operator=(move(rhs));
    delete []data;
    width =rhs.width;
    height=rhs.height;
    data=new unsigned char[width*height*4];

    data=nullptr;
    width=0;
    height=0;
   

}
;
};
int main() {
    Resource r1{"hussain"};
    
 
    return 0;
}