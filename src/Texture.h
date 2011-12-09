#ifndef Magnum_Texture_h
#define Magnum_Texture_h
/*
    Copyright © 2010, 2011 Vladimír Vondruš <mosra@centrum.cz>

    This file is part of Magnum.

    Magnum is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License version 3
    only, as published by the Free Software Foundation.

    Magnum is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License version 3 for more details.
*/

/** @file
 * @brief Class Magnum::Texture
 */

#include "AbstractTexture.h"

namespace Magnum {

/**
@brief %Texture

Template class for one- to three-dimensional textures. Recommended usage is via
subclassing and setting texture data from e.g. constructor with setData().

@attention Don't forget to call setMinificationFilter() and
setMagnificationFilter() after creating the texture, otherwise it will be
unusable.

The texture is bound via bind() and setting texture uniform on the shader to the
texture (see AbstractShaderProgram::setUniform(GLint, const AbstractTexture*)).
In shader, the texture is used via @c sampler1D, @c sampler2D or @c sampler3D
depending on dimension count. Note that you can have more than one texture bound
to the shader - the only requirement is to have each texture in another layer.

@section RectangleTextures Rectangle textures

If you want to use rectangle textures, set target in constructor to
@c GL_TEXTURE_RECTANGLE and in shader use @c sampler2DRect. Unlike @c sampler2D,
which accepts coordinates between 0 and 1, @c sampler2DRect accepts coordinates
between 0 and @c textureSizeInGivenDirection-1. Note that rectangle textures
don't support mipmapping and repeating wrapping modes, see @ref Texture::Filter
"Filter", @ref Texture::Mipmap "Mipmap" and generateMipmap() documentation
for more information.
 */
template<size_t dimensions> class Texture: public AbstractTexture {
    public:
        static const size_t Dimensions = dimensions;    /**< @brief Texture dimension count */

        /**
         * @brief Constructor
         * @param layer     Texture layer (number between 0 and 31)
         * @param target    Target, e.g. @c GL_TEXTURE_RECTANGLE. If not set,
         *      target is based on dimension count (@c GL_TEXTURE_1D,
         *      @c GL_TEXTURE_2D, @c GL_TEXTURE_3D).
         *
         * Creates one OpenGL texture.
         */
        inline Texture(GLint layer = 0, GLenum target = DataHelper<dimensions>::target()): AbstractTexture(layer, target) {}

        /**
         * @brief Set wrapping
         * @param wrapping      Wrapping type for all texture dimensions
         *
         * Sets wrapping type for coordinates out of range (0, 1) for normal
         * textures and (0, textureSizeInGivenDirection-1) for rectangle
         * textures. Note that for rectangle textures repeating wrapping modes
         * are unavailable.
         */
        void setWrapping(const Math::Vector<Wrapping, dimensions>& wrapping);

    protected:
        /**
         * @brief Set texture data
         * @param mipLevel          Mip level
         * @param internalFormat    Internal texture format
         * @param _dimensions       %Texture dimensions
         * @param colorFormat       Color format of passed data. Data size per
         *      color channel is detected from format of passed data array.
         * @param data              %Texture data
         */
        template<class T> inline void setData(GLint mipLevel, InternalFormat internalFormat, const Math::Vector<GLsizei, dimensions>& _dimensions, ColorFormat colorFormat, const T* data) {
            bind();
            DataHelper<dimensions>::template set<typename TypeTraits<T>::TextureType>(target, mipLevel, internalFormat, _dimensions, colorFormat, data);
            unbind();
        }

        /**
         * @brief Set texture subdata
         * @param mipLevel
         * @param offset            Offset where to put data in the texture
         * @param _dimensions       %Texture dimensions
         * @param colorFormat       Color format of passed data. Data size per
         *      color channel is detected from format of passed data array.
         * @param data              %Texture data
         */
        template<class T> inline void setSubData(GLint mipLevel, const Math::Vector<GLsizei, dimensions>& offset, const Math::Vector<GLsizei, dimensions>& _dimensions, ColorFormat colorFormat, const T* data) {
            bind();
            DataHelper<dimensions>::template setSub<typename TypeTraits<T>::TextureType>(target, mipLevel, offset, _dimensions, colorFormat, data);
            unbind();
        }
};

/** @brief One-dimensional texture */
typedef Texture<1> Texture1D;

/** @brief Two-dimensional texture */
typedef Texture<2> Texture2D;

/** @brief Three-dimensional texture */
typedef Texture<3> Texture3D;

}

#endif
