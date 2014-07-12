# include "../../../include/flappy_box/view/box_gl_drawable.hpp"

#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED

using namespace ::flappy_box::view;

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

BoxGlDrawable::BoxGlDrawable(const std::shared_ptr< ::flappy_box::model::Box >& b )
: _model( b )
{
	for (int i = 0; i < 128; i++) {
		for (int j = 0; j < 128; j++) {
			pixels[i][j][0] = 1.0f;
			pixels[i][j][1] = 0.0f;
			pixels[i][j][2] = 0.0f;
		}
	}

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 128, 128, 0, GL_RGB, GL_FLOAT, pixels);
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

		// enable and point arrays
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnable(GL_TEXTURE_2D);

		glVertexPointer(3, GL_FLOAT, 0, vertices);
		glNormalPointer(GL_FLOAT, 0, normals);
		glTexCoordPointer(2, GL_FLOAT, 0, texCoords);

		// draw planes
		glBindTexture(GL_TEXTURE_2D, texture);
		glDrawArrays(GL_QUADS, 0, 24);
		glDisable(GL_TEXTURE_2D);

		// switch mode for lines
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(2.0f);

		// draw lines
		glColor3d(0.2, 1.0, 0.2);
		glDrawArrays(GL_QUADS, 0, 24);

		// switch everything back to default
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glLineWidth(1.0f);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
    }
    glPopMatrix();
}
