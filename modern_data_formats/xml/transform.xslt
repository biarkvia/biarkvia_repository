<?xml version="1.0" encoding="UTF-8"?>
  <xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:output method="html" encoding="UTF-8" indent="yes"/>
  <xsl:template match="cities">
    <html> 
      <body>
        <h2>cities</h2>
        <table border="10">
          <tr bgcolor="white">
            <th style="text-left">city</th>
          </tr>
          <xsl:for-each select="city">
          <tr>
            <td><xsl:value-of select="name[@xml:lang='en']"/></td>
          </tr>
          </xsl:for-each>
        </table>
      </body>
    </html>
  </xsl:template>
</xsl:stylesheet>
