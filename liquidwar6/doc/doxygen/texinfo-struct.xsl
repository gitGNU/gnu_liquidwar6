<?xml version="1.0" standalone="no"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:template match="/doxygen/compounddef">
@deftp {Struct} {<xsl:value-of select="compoundname"/>}
<xsl:value-of select="briefdescription"/> 
<xsl:value-of select="detaileddescription"/>
@end deftp
<xsl:for-each select="/doxygen/compounddef/sectiondef/memberdef">
@defcv {Member} {<xsl:value-of select="../../compoundname"/>} {<xsl:value-of select="name"/>}
@strong{Type:} @code{<xsl:value-of select="type"/>}

@strong{Definition:} @code{<xsl:value-of select="definition"/>}

<xsl:value-of select="briefdescription"/>
<xsl:value-of select="detaileddescription"/>
@end defcv
</xsl:for-each>
</xsl:template>
</xsl:stylesheet>
