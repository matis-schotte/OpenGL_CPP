# include "../../../include/flappy_box/view/box_gl_drawable.hpp"

#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED

using namespace ::flappy_box::view;

std::vector<tinyobj::shape_t> BoxGlDrawable::shapes;

BoxGlDrawable::BoxGlDrawable(const std::shared_ptr< ::flappy_box::model::Box >& b )
: _model( b )
{
    if(shapes.empty())
    {
        std::string err = tinyobj::LoadObj(shapes, "cessna.obj", NULL);
        
        if(!err.empty())
            std::cout << std::endl << "***ERROR: " << err << std::endl;
    }
    //else
    //    for(size_t i = 0; i < shapes.size(); i++)
    //        std::cout << "Loaded obj: " << shapes[i].name << std::endl;
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
    }
    glPopMatrix();
}
