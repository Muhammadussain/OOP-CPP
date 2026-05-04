#include <iostream>
#include <string>
#include <utility>
#include <stdexcept>
#include <cstddef>
using namespace std;

class Resource {
protected:
    string name;

public:
    Resource(const std::string& n) : name(n) {}

    virtual ~Resource() = default;

    Resource(const Resource&) = default;
    Resource(Resource&&) = default;

    virtual Resource& operator=(const Resource& rhs) {
        if (this == &rhs) {
            cout << "self assignment (Resource)\n";
            return *this;
        }
        name = rhs.name;
        return *this;
    }

    virtual Resource& operator=(Resource&& rhs) {
        if (this == &rhs) return *this;
        name = std::move(rhs.name);
        rhs.name = "";
        return *this;
    }

    virtual void load() const = 0;
};

class TextureResource : public Resource {
public:
    int width = 0;
    int height = 0;
    unsigned char* data = nullptr;

    TextureResource(const string& n, int w, int h) 
        : Resource(n), width(w), height(h) {

            data = new unsigned char[width * height * 4];
    }

    ~TextureResource() override {
        delete[] data;
    }

    // Copy Constructor
    TextureResource(const TextureResource& other) 
        : Resource(other), width(other.width), height(other.height) {
        if (other.data) {
            data = new unsigned char[width * height * 4];
            std::copy(other.data, other.data + width*height*4, data);
        }
    }

    // Move Constructor
    TextureResource(TextureResource&& other) noexcept 
        : Resource(std::move(other)), 
          width(other.width), 
          height(other.height), 
          data(other.data) {
        other.data = nullptr;
        other.width = 0;
        other.height = 0;
    }

    // ===================== SIMPLE COPY ASSIGNMENT =====================
    TextureResource& operator=(const TextureResource& rhs) {
        if (this == &rhs) return *this;

        Resource::operator=(rhs);     // Base class (name) copy

        delete[] data;

        width = rhs.width;
        height = rhs.height;

        if (rhs.data) {
            data = new unsigned char[width * height * 4];
            std::copy(rhs.data, rhs.data + width*height*4, data);
        } else {
            data = nullptr;
        }

        return *this;
    }

    // Virtual Copy Assignment (Polymorphic cases ke liye)
    Resource& operator=(const Resource& rhs) override {
        const TextureResource* ptr = dynamic_cast<const TextureResource*>(&rhs);
        if (!ptr) {
            throw std::runtime_error("type mismatched");
        }

        *this = *ptr;          // Apne hi upper wale simple function ko call
        return *this;
    }

    // Move Assignment
    Resource& operator=(Resource&& rhs) override {
        TextureResource* ptr = dynamic_cast<TextureResource*>(&rhs);
        if (!ptr) throw std::runtime_error("type mismatched");

        if (this == ptr) return *this;

        Resource::operator=(std::move(rhs));

        delete[] data;
        width = ptr->width;
        height = ptr->height;
        data = ptr->data;

        ptr->data = nullptr;
        ptr->width = 0;
        ptr->height = 0;

        return *this;
    }

    void load() const override {
        cout << "Loading Texture: " << name 
             << " (" << width << "x" << height << ")" << endl;
    }

};
class Audioresource:public Resource {
    public:
    size_t SamplesCount;
    float* samples;
    Audioresource (const string& n,size_t a):Resource(n),SamplesCount(a){
         samples =new float [SamplesCount];

    };
    ~Audioresource(){
        delete [] samples;
    };
    Audioresource(const Audioresource& other):Resource(other),SamplesCount(other.SamplesCount){
        samples=new float [SamplesCount];
        copy(other.samples,other.samples+other.SamplesCount,samples);
    };
    

};

int main() {
    TextureResource t1("PlayerSprite", 512, 512);
    TextureResource t2("EnemySprite", 256, 256);

    t2 = t1;                    // Simple Copy
    t1 = std::move(t2);         // Move

    t1.load();
    t2.load();
    return 0;
}
// #include <iostream>
// #include <string>
// #include <utility>
// #include <stdexcept>

// using namespace std;

// class Resource {
// protected:
//     string name;

// public:
//     Resource(const std::string& n) : name(n) {}

//     virtual ~Resource() = default;

//     Resource(const Resource&) = default;
//     Resource(Resource&&) = default;

//     virtual Resource& operator=(const Resource& rhs) {
//         if (this == &rhs) {
//             cout << "self assignment (Resource)\n";
//             return *this;
//         }
//         name = rhs.name;
//         return *this;
//     }

//     virtual Resource& operator=(Resource&& rhs) {
//         if (this == &rhs) return *this;
//         name = std::move(rhs.name);
//         rhs.name = "";
//         return *this;
//     }

//     virtual void load() const = 0;
// };

// class TextureResource : public Resource {
// public:
//     int width = 0;
//     int height = 0;
//     unsigned char* data = nullptr;

//     // Constructor
//     TextureResource(const string& n, int w, int h) 
//         : Resource(n), width(w), height(h) {
//         if (width > 0 && height > 0) {
//             data = new unsigned char[width * height * 4];
//         }
//     }

//     // Destructor
//     ~TextureResource() override {
//         delete[] data;
//     }

//     // Copy Constructor
//     TextureResource(const TextureResource& other) 
//         : Resource(other), width(other.width), height(other.height) {
//         if (other.data) {
//             data = new unsigned char[width * height * 4];
//             std::copy(other.data, other.data + width * height * 4, data);
//         }
//     }

//     // Move Constructor
//     TextureResource(TextureResource&& other) noexcept 
//         : Resource(std::move(other)), 
//           width(other.width), 
//           height(other.height), 
//           data(other.data) {
//         other.data = nullptr;
//         other.width = 0;
//         other.height = 0;
//     }

//     // ================== Copy Assignment (TextureResource version) ==================
//     TextureResource& operator=(const TextureResource& rhs) {
//         if (this == &rhs) return *this;

//         // Call the virtual version
//         operator=(static_cast<const Resource&>(rhs));
//         return *this;                    // ← Yeh line fix hai
//     }

//     // ================== Virtual Copy Assignment (as per Requirement) ==================
//     Resource& operator=(const Resource& rhs) override {
//         const TextureResource* ptr = dynamic_cast<const TextureResource*>(&rhs);
        
//         if (!ptr) {
//             throw std::runtime_error("type mismatched");
//         }

//         if (this == ptr) return *this;

//         Resource::operator=(rhs);

//         delete[] data;

//         width = ptr->width;
//         height = ptr->height;

//         if (ptr->data) {
//             data = new unsigned char[width * height * 4];
//             std::copy(ptr->data, ptr->data + width * height * 4, data);
//         } else {
//             data = nullptr;
//         }

//         return *this;
//     }

//     // ================== Virtual Move Assignment ==================
//     Resource& operator=(Resource&& rhs) override {
//         TextureResource* ptr = dynamic_cast<TextureResource*>(&rhs);
        
//         if (!ptr) {
//             throw std::runtime_error("type mismatched");
//         }

//         if (this == ptr) return *this;

//         Resource::operator=(std::move(rhs));

//         delete[] data;

//         width = ptr->width;
//         height = ptr->height;
//         data = ptr->data;

//         ptr->data = nullptr;
//         ptr->width = 0;
//         ptr->height = 0;

//         return *this;
//     }

//     void load() const override {
//         cout << "Loading Texture: " << name 
//              << " (" << width << "x" << height << ")" << endl;
//     }
// };

// int main() {
//     TextureResource t1("PlayerSprite", 512, 512);
//     TextureResource t2("EnemySprite", 256, 256);

//     t1.load();
//     t2.load();

//     t2 = t1;                    // Copy Assignment
//     t1 = std::move(t2);         // Move Assignment

//     t1.load();

//     return 0;
// }
// #include <iostream>
// #include <string>
// #include <utility>
// #include <stdexcept>

// using namespace std;

// class Resource {
// protected:
//     string name;

// public:
//     Resource(const std::string& n) : name(n) {}

//     virtual ~Resource() = default;

//     Resource(const Resource&) = default;
//     Resource(Resource&&) = default;

//     virtual Resource& operator=(const Resource& rhs) {
//         if (this == &rhs) {
//             cout << "self assignment (Resource)\n";
//             return *this;
//         }
//         name = rhs.name;
//         return *this;
//     }

//     virtual Resource& operator=(Resource&& rhs) {
//         if (this == &rhs) return *this;
//         name = std::move(rhs.name);
//         rhs.name = "";
//         return *this;
//     }

//     // Pure Virtual Function
//     virtual void load() const = 0;
// };

// class TextureResource : public Resource {
// public:
//     int width = 0;
//     int height = 0;
//     unsigned char* data = nullptr;

//     // Constructor
//     TextureResource(const string& n, int w, int h) 
//         : Resource(n), width(w), height(h) {
//         if (width > 0 && height > 0) {
//             data = new unsigned char[width * height * 4];
//         }
//     }

//     // Destructor
//     ~TextureResource() override {
//         delete[] data;
//     }

//     // Copy Constructor
//     TextureResource(const TextureResource& other) 
//         : Resource(other), width(other.width), height(other.height) {
//         if (other.data) {
//             data = new unsigned char[width * height * 4];
//             std::copy(other.data, other.data + width * height * 4, data);
//         }
//     }

//     // Move Constructor
//     TextureResource(TextureResource&& other) noexcept 
//         : Resource(std::move(other)), 
//           width(other.width), 
//           height(other.height), 
//           data(other.data) {
//         other.data = nullptr;
//         other.width = 0;
//         other.height = 0;
//     }

//     // ============= Virtual Copy Assignment =============
//     Resource& operator=(const Resource& rhs) override {
//         const TextureResource* ptr = dynamic_cast<const TextureResource*>(&rhs);
        
//         if (!ptr) {
//             throw std::runtime_error("type mismatched");
//         }

//         if (this == &rhs) return *this;

//         Resource::operator=(rhs);        // Base class assignment

//         delete[] data;
//         width = ptr->width;
//         height = ptr->height;

//         if (ptr->data) {
//             data = new unsigned char[width * height * 4];
//             std::copy(ptr->data, ptr->data + width * height * 4, data);
//         } else {
//             data = nullptr;
//         }

//         return *this;
//     }

//     // ============= Virtual Move Assignment =============
//     Resource& operator=(Resource&& rhs) override {
//         TextureResource* ptr = dynamic_cast<TextureResource*>(&rhs);
        
//         if (!ptr) {
//             throw std::runtime_error("type mismatched");
//         }

//         if (this == &rhs) return *this;

//         Resource::operator=(std::move(rhs));   // Base move

//         delete[] data;

//         width = ptr->width;
//         height = ptr->height;
//         data = ptr->data;

//         // Leave source in valid state
//         ptr->data = nullptr;
//         ptr->width = 0;
//         ptr->height = 0;

//         return *this;
//     }

//     // Override load()
//     void load() const override {
//         cout << "Loading Texture: " << name 
//              << " (" << width << "x" << height << ")" << endl;
//     }
// };

// int main() {
//     TextureResource t1("PlayerSprite", 512, 512);
//     TextureResource t2("EnemySprite", 256, 256);

//     t1.load();
//     t2.load();

//     t2 = t1;                    // Copy Assignment (through base reference)
//     t1 = std::move(t2);         // Move Assignment

//     return 0;
// }

// hussain@hussain-Inspiron-5379:~/OOP-CPP$ g++ assignment.cpp -o ./assignment &&./assignment
// assignment.cpp: In function ‘int main()’:
// assignment.cpp:145:10: error: use of deleted function ‘TextureResource& TextureResource::operator=(const TextureResource&)’
//   145 | t2 = t1; // Copy Assignment (through base reference)
//       | ^~
// assignment.cpp:40:7: note: ‘TextureResource& TextureResource::operator=(const TextureResource&)’ is implicitly declared as deleted because ‘TextureResource’ declares a move constructor or move assignment operator
//    40 | class TextureResource : public Resource {
//       | ^~~~~~~~~~~~~~~
// assignment.cpp:146:22: error: use of deleted function ‘TextureResource& TextureResource::operator=(const TextureResource&)’
//   146 | t1 = std::move(t2); // Move Assignment
//       | ^
//  
// YEH MSLA AARH HAI




// #include <iostream>
// #include <string>
// #include <utility>
// using namespace std;

// class Resource {
// public:
//    // virtual void load() const = 0;

//     Resource(const std::string& n) : name(n) {}

//     virtual ~Resource() {}

//     Resource(const Resource&) = default;
//     Resource(Resource&&) = default;
    
//    virtual Resource& operator=(const Resource& rhs) {
//         if(this==&rhs ){
//             cout <<"self assignemnt "<<endl;
//         return *this ;
//         }
//         name =rhs.name;
//         return *this;
//     };
//     virtual Resource& operator=(Resource&& source){
//         if(this==&source)
//         {return *this ;
//         }
//         name=move(source.name);
//         source.name="";
//         return *this ;
//     };


// protected:
//     string name;
// };
// class TextureResource:public Resource{
//     public :
//     int width ;
//     int height ;
// unsigned char* data;

// TextureResource( string n,int w,int h):Resource(n),width(w),height(h){
//         data=new unsigned char[width*height*4];
// };
// ~TextureResource(){
//     delete[] data;
// };
// TextureResource(const TextureResource& other):Resource(other),width(other.width),height(other.height),data(other.data){};
// TextureResource(TextureResource&& other ):Resource(move(other)),width(other.width),height(other.height),data(other.data){
//     other.data=nullptr;
// };
// TextureResource& operator=(const Resource& rhs) override{
    
//     const TextureResource* ptr=dynamic_cast<const TextureResource*> (&rhs);

//     if(!ptr){
//         throw std::runtime_error("type mismatched");
//     }



//     if(this==&rhs) return *this;
//     Resource::operator=(rhs);
//     delete [] data;
//     width=rhs.width;
//     height=rhs.width;
//     data=new unsigned char [width*height*4];
//     for (int i=0;i<width*height*4;i++){
//         data[i]=rhs.data[i];

//     }
//     return *this;
// };
// TextureResource& operator=(TextureResource&& rhs) override{
//     TextureResource* ptr=dynamic_cast<TextureResource*>(&rhs);

//     if(!ptr){
//         throw std::runtime_error("type mis matched ");
//     }

//     if(this==&rhs) return *this ;
//     Resource::operator=(move(rhs));
//     delete []data;
//     width =rhs.width;
//     height=rhs.height;
//     data=new unsigned char[width*height*4];

//     data=nullptr;
//     width=0;
//     height=0;
   

// }
// ;
// };
// int main() {
//     Resource r1{"hussain"};
    
 
//     return 0;
// }

