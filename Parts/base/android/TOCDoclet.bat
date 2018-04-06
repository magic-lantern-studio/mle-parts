REM
REM Batch script to generate com.wizzer.mle.math API documentation.
REM This script uses the JavaTOC doclet to create an Eclipse plugin
REM for the Help system.
REM

REM The workarea, or staging area, where the documentation will be
REM generated.
SET workarea=com.wizzer.mle.parts.doc

REM Remove the workarea if it alerady exists.
IF EXIST %workarea% (
    DEL /Q %workarea%
) ELSE (
    MKDIR %workarea%
)

REM Call the JavaDOC with the JavaTOC options.
javadoc @config @options @packages

REM Generate the actual JavaDOC documentation.
SET CLASSPATH=M:\projects\MagicLantern\Core\mlert\android\com.wizzer.mle.runtime\bin;M:\projects\MagicLantern\Core\math\android\com.wizzer.mle.math\bin
CD %workarea%
CALL buildJavaDoc.bat
