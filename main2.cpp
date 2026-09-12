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
    //in place subtraction
    void operator-=(const Vec2& v){
        x-=v.x;
        y-=v.y;
    }

    //behold the holy dot product
    float Dot(const Vec2& v) const{
        return (x*v.x)+(y*v.y);
    }

    //calculates vector length
    float Length() const{
        return std::sqrt(x*x+y*y);
    }
    //makes a direction vector by shrinking it to a unit vector
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
    float mass;
    Color color;
};
//main function
int main() {
    // Open a 800x600 window
    InitWindow(800, 600, "Engine1");
    SetTargetFPS(120);

    //initializes ball objects
    Ball b1={Vec2(300.0f,100.0f), Vec2(7.0f, 0.0f), 50.0f, 45.0, WHITE};
    Ball b2={Vec2(500.0f, 100.0f), Vec2(-5.0,0.0f), 50.0f, 20.0, WHITE};
    Vec2 gravity(0.0f, 0.5f);
    // The Game Loop
    while (!WindowShouldClose()) {
        ////////////PHYSICS PART////////////
        
        //semi_implicit euler for balls calc (vel first, then pos calc) 
        b1.vel+=gravity;
        b1.pos+=b1.vel;

        b2.vel+=gravity;
        b2.pos+=b2.vel;

        //balls collision
        Vec2 delta=b2.pos-b1.pos; //calcs distance between balls
        float dist=delta.Length(); //converts that to float
        float sumRadii=b1.radius+b2.radius; //radial sum

        if(dist<sumRadii){
            Vec2 normal=delta.Normalize();

            //push apart (overlap correction)
            float overlap=sumRadii-dist;
            float invMass1=1.0f/b1.mass;
            float invMass2=1.0f/b2.mass;
            float totalInvMass=invMass1+invMass2;

            b1.pos+=normal*(overlap*(invMass1/totalInvMass));
            b2.pos+=normal*(overlap*(invMass2/totalInvMass));
            
            //bounce calculation
            Vec2 relativeVel=b2.vel-b1.vel;
            float velNormal=relativeVel.Dot(normal);

            if(velNormal<0){
                float restitution=0.8f;
                float j=-(1.0f+restitution)*velNormal;
                j=j/(totalInvMass);

                Vec2 impulse=normal*j;
                b1.vel-=impulse*(invMass1);
                b2.vel+=impulse*(invMass2);
            }
        }

        //floor collision
        float friction=0.01f;
        
        if(b1.pos.y>=(550.0f)){
            b1.pos.y=(550.0f);
            b1.vel.y=b1.vel.y*(-0.95f);
            b1.vel.x-=sgn(b1.vel.x)*friction;
            if(std::abs(b1.vel.x)<friction){
                b1.vel.x=0.0f;
            }
        }
        if(b2.pos.y >= (550.0f)) {
            b2.pos.y = (550.0f);
            b2.vel.y = b2.vel.y * (-0.95f);
            b2.vel.x -= sgn(b2.vel.x) * friction; 
            if(std::abs(b2.vel.x) < friction){ 
                b2.vel.x = 0.0f;
            } 
        }
        //wall collision
        // Left Wall b1
        if (b1.pos.x <= b1.radius) {
            b1.pos.x = b1.radius;           // Penetration resolution
            b1.vel.x = -(b1.vel.x);         // Bounce
        }
        // Right Wall b1
        if (b1.pos.x >= (800.0f - b1.radius)) {
            b1.pos.x = (800.0f - b1.radius);
            b1.vel.x = -(b1.vel.x);
        }
        // Left wall b2
        if (b2.pos.x <= b2.radius) {
            b2.pos.x = b2.radius;
            b2.vel.x = -(b2.vel.x);
        }
        // Right Wall b2
        if (b2.pos.x >= (800.0f - b2.radius)) {
            b2.pos.x = (800.0f - b2.radius);
            b2.vel.x = -(b2.vel.x);
        }
        
        ////////////RENDERING PART////////////
        BeginDrawing();
        ClearBackground({15,15,15,15});
        
        // Draw Ball 1 (With border)
        DrawCircle((int)b1.pos.x, (int)b1.pos.y, b1.radius + 3.0f, RED);
        DrawCircle((int)b1.pos.x, (int)b1.pos.y, b1.radius, b1.color);

        // Draw Ball 2 (With border)
        DrawCircle((int)b2.pos.x, (int)b2.pos.y, b2.radius + 3.0f, RED);
        DrawCircle((int)b2.pos.x, (int)b2.pos.y, b2.radius, b2.color);
        
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

