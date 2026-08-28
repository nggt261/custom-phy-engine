#include "raylib.h"
#include <cmath>

///signum function implementation
template <typename T>
inline constexpr int sgn(T val) noexcept{
    return (T(0)<val)-(val<T(0));
}
///vector maths
struct Vec2{
    float x;
    float y;

    //constructors here
    Vec2(): x(0.0f), y(0.0f) {}
    Vec2(float x, float y): x(x), y(y) {}

    //vec addition (forces combining)
    Vec2 operator+(const Vec2& v) const{
        return Vec2(x+v.x,y+v.y); //takes the x value of v (vec b) and adds it to vec a 
    }

    //vec subtraction (distance calc)
    Vec2 operator-(const Vec2& v) const{
        return Vec2(x-v.x, y-v.y);
    }

    //scalar mult (vel*deltatime)
    Vec2 operator*(float scalar) const{
        return Vec2(x*scalar, y*scalar); //takes the inputed float datatype and mult to both
    }

    //in place addition (pos+=vel)
    void operator+=(const Vec2& v){
        x+=v.x;
        y+=v.y;
    }

    //behold the holy dot product
    float Dot(const Vec2& v) const{
        return (x*v.x)+(y*v.y);
    }

    //calculates vector length
    float Length() const{
        return std::sqrt(x*x+y*y);
    }
    //makes a direction vector by shrinking it to 
    Vec2 Normalize() const{
        float len=Length();
        if(len==0.0f){
            return Vec2(0.0f,0.0f);
        }
        return Vec2(x/len, y/len);
    }
};
//creates the ball object
struct Ball{
    Vec2 pos;
    Vec2 vel;
    float radius;
    Color color;
};
int main() {
    // Open a 800x600 window
    InitWindow(800, 600, "Engine1");
    SetTargetFPS(120);

    //basic settings
    Vec2 position(400.0f, 100.0f);
    Vec2 velocity(5.0f, 0.5f); //moves down 2px/fr
    Vec2 gravity(0.0f, 0.25f);
    // The Game Loop
    while (!WindowShouldClose()) {
        ////////////PHYSICS PART////////////
        
        velocity+=gravity;      //semi implicit euler integration part
        position+=velocity;     //velocity calc first, then position
        
        //floor colision
        if(position.y>=(550.0f)){
            position.y=(550.0f);
            velocity.y=velocity.y*(-0.95f); //calculates restitution here (eg: retains 0.8times vel)

            //friction goes here
            float friction=0.1f;
            velocity.x-=sgn(velocity.x)*friction; //kinetic friction
            if(std::abs(velocity.x)<friction){
                velocity.x=0.0f; //static friction
            }
        }
        //wall collision
        if(position.x<=(50.0f) || position.x>=(750.0f)){
            if(position.x<=(50.0f)){
                position.x=(50.0f);
                velocity.x=-(velocity.x);
            }
            if(position.x>=(750.0f)){
                position.x=(750.0f);
                velocity.x=-(velocity.x);
            }
        }
        
        ////////////RENDERING PART////////////
        BeginDrawing();
        ClearBackground({15,15,15,15});
        
        DrawCircle((int)position.x, (int)position.y, 53.0f, RED);
        DrawCircle((int)position.x, (int)position.y, 50.0f, {255, 255, 255, 255});
        
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
