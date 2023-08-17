#version 330 core

uniform sampler2D texture0; // Texture input
uniform float xs,ys;            // texture resolution
uniform float r;                // blur radius

in vec2 fragTexCoord;   // texture input coord between 0 and 1.0
out vec4 fragColor;     // color output

void main()
{
    vec2 uv = (fragTexCoord- 0.5)*2.0;
    uv.x *= xs/ys; //from <-1 to +1>

    float x,y,xx,yy,rr=r*r,dx,dy,w,w0;
    w0=0.3780/pow(r,1.975);
    vec2 p;
    vec4 col=vec4(0.0,0.0,0.0,0.0);
    for (dx=1.0/xs,x=-r,p.x=0.5+(fragTexCoord.x*0.5)+(x*dx);x<=r;x++,p.x+=dx){ xx=x*x;
     for (dy=1.0/ys,y=-r,p.y=0.5+(fragTexCoord.y*0.5)+(y*dy);y<=r;y++,p.y+=dy){ yy=y*y;
      if (xx+yy<=rr)
        {
        w=w0*exp((-xx-yy)/(2.0*rr));
        col+=texture(texture0,p)*w;
        }}}
    fragColor=col;
}
