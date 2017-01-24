/****************************************************************************
* This is the Computer Graphics Shader Lab.
*
* Copyright (c) 2016 Bernhard Kainz, Antoine S Toisoul
* (b.kainz@imperial.ac.uk, antoine.toisoul13@imperial.ac.uk)
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
* IN THE SOFTWARE.
****************************************************************************/

//=============================================================================
/** @file		HighLighter.cpp
 *
 * Implements a GLSL syntax highlighter.
 *
    Shader Maker - a cross-platform GLSL editor.
    Copyright (C) 2007-2008 Markus Kramer
    For details, see main.cpp or COPYING.

=============================================================================*/

#include "GLSLCodeEditor.h"

//=============================================================================
//	CGlSlHighlighter definition
//=============================================================================

/** A syntax highlighter for GLSL.
 * This is mainly the modified syntax highlighter example from the Qt toolkit.
 * Because GLSL has really many special worlds (keyworlds, built-in uniforms,
 * attributes, ... ) this syntax highlighter operates quite slow. A possible
 * improvement would be to reduce the number of supported keywords, but that
 * would confuse the user.
 */
class CGlSlHighlighter : public QSyntaxHighlighter
{
public:
    /** Constructs a GLSL syntax highlighter object.
     * @param doc Document to highlight.
     */
    CGlSlHighlighter(QTextDocument* doc);

protected:
    /** Does actual highlighting.
     * See Qt documentation for details.
     */
    void highlightBlock(const QString & text);

private:

    // construction hlpers
    void setupKeywords(void);
    void setupReservedKeywords(void);
    void setupComments(void);
    void setupNumberRules(void);
    void setupRegisters(void);
    void setupBuiltInFunctions(void);
    void setupPreprocessor(void);

    // rules
    typedef struct highlightRule_s {
        QRegExp pattern;
        QTextCharFormat format;
    } highlightRule_t;
    QVector< highlightRule_t > m_rules;

    // comment expressions
    QRegExp m_commentStartExpression;
    QRegExp m_commentEndExpression;

    // format infos
    QTextCharFormat m_keywordFormat;
    QTextCharFormat m_reservedKeywordFormat;
    QTextCharFormat m_integerFormat;
    QTextCharFormat m_floatFormat;
    QTextCharFormat m_registerFormat; // IO registers and GL uniform state
    QTextCharFormat m_builtInFunctionFormat;
    QTextCharFormat m_preprocessorFormat;
    QTextCharFormat m_singleLineCommentFormat;
    QTextCharFormat m_multiLineCommentFormat;
};


/*
========================
GLSLCodeEditor::createSyntaxHighlighter

 object glue
========================
*/
void GLSLCodeEditor::createSyntaxHighlighter(void)
{
    m_highlighter = new CGlSlHighlighter(document());
}


//=============================================================================
//	implementation
//=============================================================================

// construction
CGlSlHighlighter::CGlSlHighlighter(QTextDocument* doc)
    : QSyntaxHighlighter(doc)
{
    setupKeywords();
    setupReservedKeywords();
    setupPreprocessor();
    setupNumberRules();
    setupRegisters();
    setupBuiltInFunctions();
    setupComments();
}


/*
========================
highlightBlock
========================
*/
void CGlSlHighlighter::highlightBlock(const QString & text)
{
    // check out all rules
    foreach(highlightRule_t rule, m_rules)
    {
        QRegExp expression(rule.pattern);
        int index = text.indexOf(expression);

        while (index >= 0)
        {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = text.indexOf(expression, index + length);
        }
    }

    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
    {
        startIndex = text.indexOf(m_commentStartExpression);
    }

    while (startIndex >= 0)
    {
        int endIndex = text.indexOf(m_commentEndExpression, startIndex);
        int commentLength;

        if (endIndex == -1)
        {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        }
        else
        {
            commentLength = endIndex - startIndex
                + m_commentEndExpression.matchedLength();
        }

        setFormat(startIndex, commentLength, m_multiLineCommentFormat);
        startIndex = text.indexOf(m_commentStartExpression, startIndex + commentLength);
    }
}


/*
========================
setupComments
========================
*/
void CGlSlHighlighter::setupComments(void)
{
    highlightRule_t rule;

    m_singleLineCommentFormat.setForeground(Qt::darkGreen);
    m_singleLineCommentFormat.setFontItalic(true);

    rule.pattern = QRegExp("//[^\n]*");
    rule.format = m_singleLineCommentFormat;
    m_rules.append(rule);

    m_multiLineCommentFormat.setForeground(Qt::darkGreen);
    m_multiLineCommentFormat.setFontItalic(true);

    m_commentStartExpression = QRegExp("/\\*");
    m_commentEndExpression = QRegExp("\\*/");
}


/*
========================
setupPreprocessor
========================
*/
void CGlSlHighlighter::setupPreprocessor(void)
{
    m_preprocessorFormat.setForeground(Qt::blue);

    // highlight each line beginning with #
    highlightRule_t rule;
    rule.pattern = QRegExp("#[^\n]*");
    rule.format = m_preprocessorFormat;
    m_rules.append(rule);
}


/*
========================
setupNumberRules
========================
*/
void CGlSlHighlighter::setupNumberRules(void)
{
    highlightRule_t rule;

    m_integerFormat.setForeground(Qt::magenta);
    m_floatFormat.setForeground(Qt::darkBlue);

    rule.pattern = QRegExp("\\b[0-9]+\\b");
    rule.format = m_integerFormat;
    m_rules.append(rule);

    // must be appended after integer rule to "overwrite" it.
    rule.pattern = QRegExp("\\b[0-9]\\.[0-9]+\\b");
    rule.format = m_floatFormat;
    m_rules.append(rule);
}


/*
========================
setupRegisters
========================
*/
void CGlSlHighlighter::setupRegisters(void)
{
    highlightRule_t rule;
    QStringList keywordPatterns;

    m_registerFormat.setForeground(Qt::darkBlue);
    m_registerFormat.setFontWeight(QFont::Bold);

    //
    // built-in uniforms/attributes
    //
    keywordPatterns << "\\bgl_Position\\b" << "\\bgl_PointSize\\b" << "\\bgl_ClipVertex\\b"
        << "\\bgl_FragCoord\\b" << "\\bgl_FrontFacing\\b" << "\\bgl_FragColor\\b"
        << "\\bgl_FragData\\b" << "\\bgl_FragDepth\\b"
        << "\\bgl_Color\\b" << "\\bgl_SecondaryColor\\b" << "\\bgl_Normal\\b"
        << "\\bgl_Vertex\\b" << "\\bgl_MultiTexCoord0\\b" << "\\bgl_MultiTexCoord1\\b"
        << "\\bgl_MultiTexCoord2\\b" << "\\bgl_MultiTexCoord3\\b" << "\\bgl_MultiTexCoord4\\b"
        << "\\bgl_MultiTexCoord5\\b" << "\\bgl_MultiTexCoord6\\b" << "\\bgl_MultiTexCoord7\\b"
        << "\\bgl_FogCoord\\b"
        // impl. maxima
        << "\\bgl_MaxLights\\b"
        << "\\bgl_MaxClipPlanes\\b"
        << "\\bgl_MaxTextureUnits\\b"
        << "\\bgl_MaxtextureCoords\\b"
        << "\\bgl_MaxVertexAttribs\\b"
        << "\\bgl_MaxVertexUniformComponents\\b"
        << "\\bgl_MaxVaryingFloats\\b"
        << "\\bgl_MaxVertexTextureImageUnits\\b"
        << "\\bgl_MaxCombinedtextureImageUnits\\b"
        << "\\bgl_MaxTextureImageUnits\\b"
        << "\\bgl_MaxFragmentUniformComponents\\b"
        << "\\bgl_MaxDrawBuffers\\b"
        // matrices
        << "\\bgl_ModelViewMatrix\\b"
        << "\\bgl_ProjectionMatrix\\b"
        << "\\bgl_ModelViewProjectionMatrix\\b"
        << "\\bgl_TextureMatrix\\b"
        << "\\bgl_NormalMatrix\\b"
        << "\\bgl_ModelViewMatrixInverse\\b"
        << "\\bgl_ProjectionMatrixInverse\\b"
        << "\\bgl_ModelViewProjectionMatrixInverse\\b"
        << "\\bgl_TextureMatrixInverse\\b"
        << "\\bgl_ModelViewMatrixTranspose\\b"
        << "\\bgl_ProjectionMatrixTranspose\\b"
        << "\\bgl_ModelViewProjectionMatrixTranspose\\b"
        << "\\bgl_TextureMatrixTranspose\\b"
        << "\\bgl_ModelViewMatrixInverseTranspose\\b"
        << "\\bgl_ProjectionMatrixInverseTranspose\\b"
        << "\\bgl_ModelViewProjectionMatrixInverseTranspose\\b"
        << "\\bgl_TextureMatrixInverseTranspose\\b"
        // misc state
        << "\\bgl_NormalScale\\b"
        << "\\bgl_DepthRangeparameters\\b"
        << "\\bgl_DepthRange\\b"
        << "\\bgl_ClipPlane\\b"
        << "\\bgl_PointParameters\\b"
        << "\\bgl_Point\\b"
        << "\\bgl_MaterialParameters\\b"
        << "\\bgl_FrontMaterial\\b"
        << "\\bgl_BackMaterial\\b"
        << "\\bgl_LightSourceParameters\\b"
        << "\\bgl_LightSource\\b"
        << "\\bgl_LightModelParameters\\b"
        << "\\bgl_LightModel\\b"
        << "\\bgl_LightModelProducts\\b"
        << "\\bgl_FrontLightModelProduct\\b"
        << "\\bgl_BackLightModelProduct\\b"
        << "\\bgl_LightProducts\\b"
        << "\\bgl_FrontLightProduct\\b"
        << "\\bgl_BackLightProduct\\b"
        << "\\bgl_TextureEnvColor\\b"
        << "\\bgl_EyePlaneS\\b"
        << "\\bgl_EysPlaneT\\b"
        << "\\bgl_EyePlaneR\\b"
        << "\\bgl_EyePlaneQ\\b"
        << "\\bgl_ObjectPlaneS\\b"
        << "\\bgl_ObjectPlaneT\\b"
        << "\\bgl_ObjectPlaneR\\b"
        << "\\bgl_ObjectPlaneQ\\b"
        << "\\bgl_FogParameters\\b"
        << "\\bgl_Fog\\b"
        // varying vars
        << "\\bgl_FrontColor\\b"
        << "\\bgl_BackColor\\b"
        << "\\bgl_FrontSecondaryColor\\b"
        << "\\bgl_BackSecondaryColor\\b"
        << "\\bgl_TexCoord\\b"
        << "\\bgl_FogFragCoord\\b"
        << "\\bgl_PointCoord\\b"
        // geometry shader
        << "\\bgl_FrontColorIn\\b"
        << "\\bgl_BackColorIn\\b"
        << "\\bgl_FrontSecondaryColorIn\\b"
        << "\\bgl_BackSecondaryColorIn\\b"
        << "\\bgl_TexCoordIn\\b"
        << "\\bgl_VerticesIn\\b"
        << "\\bgl_FogFragCoordIn\\b"
        << "\\bgl_PositionIn\\b"
        << "\\bgl_PointSizeIn\\b"
        << "\\bgl_ClipVertexIn\\b"
        ;

    foreach(QString pattern, keywordPatterns)
    {
        rule.pattern = QRegExp(pattern);
        rule.format = m_registerFormat;
        m_rules.append(rule);
    }
}


/*
========================
setupBuiltInFunctions
========================
*/
void CGlSlHighlighter::setupBuiltInFunctions(void)
{
    highlightRule_t rule;
    QStringList keywordPatterns;

    m_builtInFunctionFormat.setForeground(Qt::darkBlue);
    m_builtInFunctionFormat.setFontWeight(QFont::Bold);


    keywordPatterns // trigonometry
        << "\\bradians\\b"
        << "\\bdegrees\\b"
        << "\\bsin\\b"
        << "\\bcos\\b"
        << "\\btan\\b"
        << "\\basin\\b"
        << "\\bacos\\b"
        << "\\batan\\b"
        // exponential
        << "\\bpow\\b"
        << "\\bexp\\b"
        << "\\blog\\b"
        << "\\bexp2\\b"
        << "\\blog2\\b"
        << "\\bsqrt\\b"
        << "\\binversesqrt\\b"
        // common
        << "\\babs\\b"
        << "\\bsign\\b"
        << "\\bfloor\\b"
        << "\\bceil\\b"
        << "\\bfract\\b"
        << "\\bmod\\b"
        << "\\bmin\\b"
        << "\\bmax\\b"
        << "\\bclamp\\b"
        << "\\bmix\\b"
        << "\\bstep\\b"
        << "\\bsmoothstep\\b"
        << "\\blength\\b"
        << "\\bdistance\\b"
        << "\\bdot\\b"
        << "\\bcross\\b"
        << "\\bnormalize\\b"
        << "\\bftransform\\b"
        << "\\bfaceforward\\b"
        << "\\breflect\\b"
        << "\\brefract\\b"
        // matrices
        << "\\bmatrixCompMult\\b"
        << "\\bouterProduct\\b"
        << "\\btranspose\\b"
        << "\\binverse\\b"
        // vectors
        << "\\blessThan\\b"
        << "\\blessThanEqual\\b"
        << "\\bgreaterThan\\b"
        << "\\bgreaterThanEqual\\b"
        << "\\bequal\\b"
        << "\\bnotEqual\\b"
        << "\\bany\\b"
        << "\\ball\\b"
        << "\\bnot\\b"
        // texturing
        << "\\btexture1D\\b"
        << "\\btexture1DProj\\b"
        << "\\btexture1DLod\\b"
        << "\\btexture1DProjLod\\b"
        << "\\btexture2D\\b"
        << "\\btexture2DProj\\b"
        << "\\btexture2DLod\\b"
        << "\\btexture2DProjLod\\b"
        << "\\btexture3D\\b"
        << "\\btexture3DProj\\b"
        << "\\btexture3DLod\\b"
        << "\\btexture3DProjLod\\b"
        << "\\btextureCube\\b"
        << "\\btextureCubeLod\\b"
        << "\\bshadow1D\\b"
        << "\\bshadow2D\\b"
        << "\\bshadow1DProj\\b"
        << "\\bshadow2DProj\\b"
        << "\\bshadow1DLod\\b"
        << "\\bshadow2DLod\\b"
        << "\\bshadow1DProjLod\\b"
        << "\\bshaodw2DProjLod\\b"
        // fragment processing
        << "\\bdFdx\\b"
        << "\\bdFdy\\b"
        << "\\bfwidth\\b"
        // noise
        << "\\bnoise1\\b"
        << "\\bnoise2\\b"
        << "\\bnoise3\\b"
        << "\\bnoise4\\b"
        // geometry shader
        << "\\bEmitVertex\\b"
        << "\\bEndPrimitive\\b"
        ;

    foreach(QString pattern, keywordPatterns)
    {
        rule.pattern = QRegExp(pattern);
        rule.format = m_builtInFunctionFormat;
        m_rules.append(rule);
    }
}


/*
========================
setupKeywords
========================
*/
void CGlSlHighlighter::setupKeywords(void)
{
    highlightRule_t rule;
    QStringList keywordPatterns;

    m_keywordFormat.setForeground(Qt::blue);

    //
    // used keywords
    //
    keywordPatterns << "\\battribute\\b" << "\\bconst\\b" << "\\buniform\\b"
        << "\\bvarying\\b" << "\\bcentroid\\b" << "\\bbreak\\b"
        << "\\bcontinue\\b" << "\\bdo\\b" << "\\bfor\\b"
        << "\\bwhile\\b" << "\\bif\\b" << "\\belse\\b"
        << "\\bin\\b" << "\\bout\\b" << "\\binout\\b"
        << "\\bfloat\\b" << "\\bint\\b" << "\\bvoid\\b"
        << "\\bbool\\b" << "\\btrue\\b" << "\\bfalse\\b"
        << "\\binvariant\\b"
        << "\\bdiscard\\b" << "\\breturn\\b"
        << "\\bmat2\\b" << "\\bmat3\\b" << "\\bmat4\\b"
        << "\\bmat2x2\\b" << "\\bmat2x3\\b" << "\\bmat2x4\\b"
        << "\\bmat3x2\\b" << "\\bmat3x3\\b" << "\\bmat3x4\\b"
        << "\\bmat4x2\\b" << "\\bmat4x3\\b" << "\\bmat4x4\\b"
        << "\\bvec2\\b" << "\\bvec3\\b" << "\\bvec4\\b"
        << "\\bivec2\\b" << "\\bivec3\\b" << "\\bivec4\\b"
        << "\\bbvec2\\b" << "\\bbvec3\\b" << "\\bbvec4\\b"
        << "\\bsampler1D\\b" << "\\bsampler2D\\b" << "\\bsampler3D\\b"
        << "\\bsamplerCube\\b" << "\\bsampler1DShadow\\b" << "\\bsampler2DShadow\\b"
        << "\\bstruct\\b";

    foreach(QString pattern, keywordPatterns)
    {
        rule.pattern = QRegExp(pattern);
        rule.format = m_keywordFormat;
        m_rules.append(rule);
    }
}


/*
========================
setupReservedKeywords
========================
*/
void CGlSlHighlighter::setupReservedKeywords(void)
{
    highlightRule_t rule;
    QStringList keywordPatterns;

    m_reservedKeywordFormat.setForeground(Qt::red);
    m_reservedKeywordFormat.setFontWeight(QFont::Bold);

    //
    // reserved keywords
    //
    keywordPatterns << "\\basm\\b" << "\\bclass\\b" << "\\bunion\\b"
        << "\\benum\\b" << "\\btypedef\\b" << "\\btemplate\\b"
        << "\\bthis\\b" << "\\bpacked\\b" << "\\bgoto\\b"
        << "\\bswitch\\b" << "\\bdefault\\b" << "\\binline\\b"
        << "\\bnoinline\\b" << "\\bvolatile\\b" << "\\bpublic\\b"
        << "\\bstatic\\b" << "\\bextern\\n" << "\\bexternal\\b"
        << "\\binterface\\b" << "\\blong\\b" << "\\bshort\\b"
        << "\\bdouble\\b" << "\\bhalf\\b" << "\\bfixed\\b"
        << "\\bunsigned\\b" << "\\blowp\\b" << "\\bmediump\\b"
        << "\\bhighp\\b" << "\\bprecision\\b"
        << "\\binput\\b" << "\\boutput\\b"
        << "\\bhvec2\\b" << "\\bhvec3\\b" << "\\bhvec4\\b"
        << "\\bdvec2\\b" << "\\bdvec3\\b" << "\\bdvec4\\b"
        << "\\bfvec2\\b" << "\\bfvec3\\b" << "\\bfvec4\\b"
        << "\\bsampler2DRect\\b" << "\\bsampler3DRect\\b" << "\\bsampler2DRectShadow\\b"
        << "\\bsizeof\\b" << "\\bcast\\b" << "\\bnamespace\\b" << "\\busing\\b";

    foreach(QString pattern, keywordPatterns)
    {
        rule.pattern = QRegExp(pattern);
        rule.format = m_reservedKeywordFormat;
        m_rules.append(rule);
    }
}






