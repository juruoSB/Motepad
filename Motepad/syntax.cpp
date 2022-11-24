#include "syntaxClassic.h"

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    keywordFormat.setForeground(QBrush(QColor(197,39,39)));
    QStringList keywordPatterns; //关键词
    keywordPatterns<<"\\balignas\\b"<<"\\bcontinue\\b"<<"\\bfriend\\b"<<"\\bregister\\b"<<"\\btrue\\b"<<"\\balignof\\b"<<"\\bdecltype\\b"<<"\\bgoto\\b"<<"\\breinterpret_cast\\b"<<"\\btry\\b"<<"\\basm\\b"<<"\\bdefault\\b"<<"\\bif\\b"<<"\\breturn\\b"<<"\\btypedef\\b"<<"\\bauto\\b"<<"\\bdelete\\b"<<"\\binline\\b"<<"\\bshort\\b"<<"\\btypeid\\b"<<"\\bbool\\b"<<"\\bdo\\b"<<"\\bint\\b"<<"\\bsigned\\b"<<"\\btypename\\b"<<"\\bbreak\\b"<<"\\bdouble\\b"<<"\\blong\\b"<<"\\bsizeof\\b"<<"\\bunion\\b"<<"\\bcase\\b"<<"\\bdynamic_cast\\b"<<"\\bmutable\\b"<<"\\bstatic\\b"<<"\\bunsigned\\b"<<"\\bcatch\\b"<<"\\belse\\b"<<"\\bnamespace\\b"<<"\\bstatic_assert\\b"<<"\\busing\\b"<<"\\bchar\\b"<<"\\benum\\b"<<"\\bnew\\b"<<"\\bstatic_cast\\b"<<"\\bvirtual\\b"<<"\\bchar16_t\\b"<<"\\bexplicit\\b"<<"\\bnoexcept\\b"<<"\\bstruct\\b"<<"\\bvoid\\b"<<"\\bchar32_t\\b"<<"\\bexport\\b"<<"\\bnullptr\\b"<<"\\bswitch\\b"<<"\\bvolatile\\b"<<"\\bclass\\b"<<"\\bextern\\b"<<"\\boperator\\b"<<"\\btemplate\\b"<<"\\bwchar_t\\b"<<"\\bconst\\b"<<"\\bfalse\\b"<<"\\bprivate\\b"<<"\\bthis\\b"<<"\\bwhile\\b"<<"\\bconstexpr\\b"<<"\\bfloat\\b"<<"\\bprotected\\b"<<"\\bthread_local\\b"<<"\\bconst_cast\\b"<<"\\bfor\\b"<<"\\bpublic\\b"<<"\\bthrow\\b";

    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }
    singleLineCommentFormat.setForeground(QBrush(QColor(194,200,186)));//单行注释
    rule.pattern = QRegularExpression("//[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(QBrush(QColor(194,200,186)));//多行注释
    numFormat.setForeground(QBrush(QColor(70,166,9)));// 数字
    rule.pattern = QRegularExpression
("\\b-?(((((0|[1-9][0-9]*))\\.[0-9]*))|[1-9][0-9]*|0[1-7][0-7]*|0x([1-9]|[A-F]|[a-f])([0-9]|[A-F]|[a-f])*|0|00|0x0)\\b");
    rule.format = numFormat;
    highlightingRules.append(rule);
    quotationFormat.setForeground(QBrush(QColor(0,47,167)));//字符串
    rule.pattern = QRegularExpression("\".*\"|<.*>|'.'");
    rule.format = quotationFormat;
    highlightingRules.append(rule);
    functionFormat.setFontItalic(true);//函数
    functionFormat.setForeground(QBrush(QColor(192,149,111)));
    rule.pattern = QRegularExpression("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);
    quotationFormat.setForeground(QBrush(QColor(182,149,111)));//预处理器关键字
    rule.pattern = QRegularExpression
(" *(#|%:|\\?\\?=) *(include|define|if|ifdef|ifndef|else|elif|endif|undef|pragma|error|line|warning).*");
    rule.format = quotationFormat;
    highlightingRules.append(rule);
    commentStartExpression = QRegularExpression("/\\*");//多行注释的匹配正则表达式
    commentEndExpression = QRegularExpression("\\*/");
}

void Highlighter::highlightBlock(const QString &text)//应用高亮规则
{
    foreach (const HighlightingRule &rule, highlightingRules) {//文本采用高亮规则
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
    setCurrentBlockState(0); //多行注释的匹配
    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(commentStartExpression);

    while (startIndex >= 0) {
        QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                            + match.capturedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
    }


}
