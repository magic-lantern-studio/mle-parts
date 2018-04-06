REM
REM Batch script to generate com.wizzer.mle.parts.roles API documentation.
REM This script uses the JavaTOC doclet to create an Eclipse plugin
REM for the Help system.
REM

REM The workarea, or staging area, where the documentation will be
REM generated.
SET workarea=com.wizzer.mle.parts.roles.doc

REM Remove the workarea if it alerady exists.
IF EXIST %workarea% (
    DEL /Q %workarea%
) ELSE (
    MKDIR %workarea%
)

REM Call the JavaDOC with the JavaTOC options.
javadoc @config @options @packages

REM Generate the actual JavaDOC documentation.
SET CLASSPATH=M:\projects\MagicLantern\Core\mlert\java\com.wizzer.mle.runtime\bin;M:\projects\MagicLantern\Core\math\java\com.wizzer.mle.math\bin;M:\projects\MagicLantern\Parts\base\java\com.wizzer.mle.parts\bin;M:\projects\MagicLantern\Parts\props\java\com.wizzer.mle.parts.props\bin;M:\projects\MagicLantern\Parts\mediaref\java\com.wizzer.mle.parts.mrefs\bin
CD %workarea%
CALL buildJavaDoc.bat
