# include "../../../include/flappy_box/view/box_gl_drawable.hpp"

using namespace ::flappy_box::view;

std::vector<tinyobj::shape_t> BoxGlDrawable::shapes;

// all points of the cube
#define POINT_A -.5,+.5,-.5
#define POINT_B +.5,+.5,-.5
#define POINT_C +.5,-.5,-.5
#define POINT_D -.5,-.5,-.5
#define POINT_E -.5,+.5,+.5
#define POINT_F +.5,+.5,+.5
#define POINT_G +.5,-.5,+.5
#define POINT_H -.5,-.5,+.5

// all normals of the cube
#define NORMAL_ABCD 0,0,-1
#define NORMAL_AEFB 0,+1,0
#define NORMAL_ADHE -1,0,0
#define NORMAL_FGCB +1,0,0
#define NORMAL_HGFE 0,0,+1
#define NORMAL_GHDC 0,-1,0

// texture coordinates
#define TEXCOORD_UL 0,0
#define TEXCOORD_UR 0,1
#define TEXCOORD_DR 1,1
#define TEXCOORD_DL 1,0

BoxGlDrawable::BoxGlDrawable(const std::shared_ptr< ::flappy_box::model::Box >& b ) : _model( b )
{
    if(shapes.empty() && false) // no obj loading needed here
    {
        std::string err = tinyobj::LoadObj(shapes, "cessna.obj", NULL);
        
        if(!err.empty())
            std::cout << std::endl << "***ERROR: " << err << std::endl;
    }
    //else
    //    for(size_t i = 0; i < shapes.size(); i++)
    //        std::cout << "Loaded obj: " << shapes[i].name << std::endl;
    
	FILE *rawFile = fopen("thehead.raw", "rb");
	fread(pixels, sizeof(pixels[0]), sizeof(pixels) / sizeof(pixels[0]), rawFile);
	fclose(rawFile);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 128, 128, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
}

BoxGlDrawable::~BoxGlDrawable()
{
	glBindTexture(GL_TEXTURE_2D, NULL);
	glDeleteTextures(1, &texture);
}

void BoxGlDrawable::visualize( ::view::GlRenderer& r, ::view::GlutWindow& w )
{    
    glPushMatrix();
    {
		// translate, rotate an scale
        double angle = _model->angle();
        const vec3_type& pos = _model->position();
        const double size = _model->size();
        glTranslated( pos(0), pos(1), pos(2) );
        glRotated( angle, 1., 1., 0. );
        glScaled( size, size, size );

		// define vertices, normals and texture coordinates
		const GLfloat vertices[] = {
			POINT_A, POINT_B, POINT_C, POINT_D,
			POINT_A, POINT_E, POINT_F, POINT_B,
			POINT_A, POINT_D, POINT_H, POINT_E,
			POINT_F, POINT_G, POINT_C, POINT_B,
			POINT_H, POINT_G, POINT_F, POINT_E,
			POINT_G, POINT_H, POINT_D, POINT_C
		};
		const GLfloat normals[] = {
			NORMAL_ABCD, NORMAL_ABCD, NORMAL_ABCD, NORMAL_ABCD,
			NORMAL_AEFB, NORMAL_AEFB, NORMAL_AEFB, NORMAL_AEFB,
			NORMAL_ADHE, NORMAL_ADHE, NORMAL_ADHE, NORMAL_ADHE,
			NORMAL_FGCB, NORMAL_FGCB, NORMAL_FGCB, NORMAL_FGCB,
			NORMAL_HGFE, NORMAL_HGFE, NORMAL_HGFE, NORMAL_HGFE,
			NORMAL_GHDC, NORMAL_GHDC, NORMAL_GHDC, NORMAL_GHDC
		};
		const GLfloat texCoords[] = {
			TEXCOORD_UL, TEXCOORD_UR, TEXCOORD_DR, TEXCOORD_DL,
			TEXCOORD_UL, TEXCOORD_UR, TEXCOORD_DR, TEXCOORD_DL,
			TEXCOORD_UL, TEXCOORD_UR, TEXCOORD_DR, TEXCOORD_DL,
			TEXCOORD_UL, TEXCOORD_UR, TEXCOORD_DR, TEXCOORD_DL,
			TEXCOORD_UL, TEXCOORD_UR, TEXCOORD_DR, TEXCOORD_DL,
			TEXCOORD_UL, TEXCOORD_UR, TEXCOORD_DR, TEXCOORD_DL
		};

		// enable and point to arrays
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glVertexPointer(3, GL_FLOAT, 0, vertices);
		glNormalPointer(GL_FLOAT, 0, normals);
		glTexCoordPointer(2, GL_FLOAT, 0, texCoords);

		// draw planes
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);
		glColor3d(0.4, 0.8, 0.4);
		glDrawArrays(GL_QUADS, 0, 24);
		glDisable(GL_TEXTURE_2D);

		// switch mode, line width and color for line drawing
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(2.0f);
		glColor3d(0.2, 1.0, 0.2);
		glDrawArrays(GL_QUADS, 0, 24);

		// switch everything back to default
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glLineWidth(1.0f);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        
        /*
        // modell besser platzieren
        glRotated( 90., 1., 1., 0. );
        glScaled( .2, .2, .2 );
        
        //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        
        glColor3d( .4, .8, .4 );
        
        ::glBegin(GL_TRIANGLES); // GL_LINES GL_TRIANGLES
        
        for(size_t i = 0; i < shapes.size(); i++)
            for(size_t f = 0; f < shapes[i].mesh.indices.size(); f++)
            {
                if((f%3) == 0) // berechnen der normalen
                {
                    vec3_type a(shapes[i].mesh.positions[3*shapes[i].mesh.indices[f]+0], shapes[i].mesh.positions[3*shapes[i].mesh.indices[f]+1], shapes[i].mesh.positions[3*shapes[i].mesh.indices[f]+2]), b(shapes[i].mesh.positions[3*shapes[i].mesh.indices[f+1]+0], shapes[i].mesh.positions[3*shapes[i].mesh.indices[f+1]+1], shapes[i].mesh.positions[3*shapes[i].mesh.indices[f+1]+2]), c(shapes[i].mesh.positions[3*shapes[i].mesh.indices[f+2]+0], shapes[i].mesh.positions[3*shapes[i].mesh.indices[f+2]+1], shapes[i].mesh.positions[3*shapes[i].mesh.indices[f+2]+2]), d(0,0,0), e(0,0,0), final(0,0,0);
                    d = a-b; e = a-c;
                    final = d.cross(e); final.normalize();
                    ::glNormal3d(final(0), final(1), final(2));
                }
                ::glVertex3d(shapes[i].mesh.positions[3*shapes[i].mesh.indices[f]+0], shapes[i].mesh.positions[3*shapes[i].mesh.indices[f]+1], shapes[i].mesh.positions[3*shapes[i].mesh.indices[f]+2]);
            }
        
        / *
         ::glVertex3d(-0.5, -0.5, -0.5);
         ::glVertex3d(-0.5, -0.5,  0.5);
         ::glVertex3d(-0.5,  0.5, -0.5);
         ::glVertex3d(-0.5,  0.5,  0.5);
         ::glVertex3d( 0.5, -0.5, -0.5);
         ::glVertex3d( 0.5, -0.5,  0.5);
         ::glVertex3d( 0.5,  0.5, -0.5);
         ::glVertex3d( 0.5,  0.5,  0.5);
         
         ::glVertex3d(-0.5, -0.5, -0.5);
         ::glVertex3d(-0.5,  0.5, -0.5);
         ::glVertex3d( 0.5, -0.5, -0.5);
         ::glVertex3d( 0.5,  0.5, -0.5);
         ::glVertex3d(-0.5, -0.5,  0.5);
         ::glVertex3d(-0.5,  0.5,  0.5);
         ::glVertex3d( 0.5, -0.5,  0.5);
         ::glVertex3d( 0.5,  0.5,  0.5);
         
         ::glVertex3d(-0.5, -0.5, -0.5);
         ::glVertex3d( 0.5, -0.5, -0.5);
         ::glVertex3d(-0.5, -0.5,  0.5);
         ::glVertex3d( 0.5, -0.5,  0.5);
         ::glVertex3d(-0.5,  0.5, -0.5);
         ::glVertex3d( 0.5,  0.5, -0.5);
         ::glVertex3d(-0.5,  0.5,  0.5);
         ::glVertex3d( 0.5,  0.5,  0.5);
         * /
        
        ::glEnd();
        */
    }
    glPopMatrix();
}
