#pragma once

#define NODE_STYLE 1

namespace EMIR
{

class OPS_TIKZ
{

public:

    const char* TIKZ_HEADER = {
        "\\documentclass{standalone}               \n\n \
\\usepackage{tikz}                       \n \
                                                    \n \
\\usetikzlibrary{arrows,positioning}     \n \
\\usetikzlibrary{scopes}                 \n \
\\usetikzlibrary{backgrounds}            \n \
                                                    \n \
\\pgfdeclarelayer{nodelayer}             \n \
\\pgfdeclarelayer{edgelayer}             \n \
\\pgfsetlayers{edgelayer,nodelayer,main} \n \n"
    };

    const char* TIKZ_OPENING = {
        "\\begin{document} \n\n"
    };


    const char* TIKZ_CLOSING = {
        "\n\\end{document} \n"
    };


    const char* TIKZ_FIG_HEADER = {
        "\\begin{tikzpicture}                                 \n\n \
\\tikzstyle{nosty} = [draw=none,fill=none,right] \n \
\\tikzstyle{slit}  = [thick]                     \n \n"
    };

    const char* TIKZ_FIG_CLOSING = {
        "\n\\end{tikzpicture}                                   \n"
    };

    const char* TIKZ_GANTT_HEADER = {
        "\\newcommand{\\openslit}[2]{                                        \n \
                                                                               \n \
                    \\node [nosty] (0) at (-4.5, 3 - #1) {};                    \n \
                    \\node [nosty] (1) at (-3.5, 3 - #1) {};                    \n \
                    \\node [nosty] (2) at (-3.5, 3 - #1 - #2) {};               \n \
                    \\node [nosty] (3) at (-4.5, 3 - #1 - #2) {};               \n \
                    \\node [nosty] (4) at (-5.0, 3 - #1 - #2 / 2) {$#1$};       \n \
                                                                               \n \
                    \\draw[slit] (1.center) to (0.center);                      \n \
                    \\draw[slit] (1.center) to (2.center);                      \n \
                    \\draw[slit] (2.center) to (3.center);	                   \n \
                                                                               \n \
                    \\node [nosty] (0) at (4.5, 3 - #1) {};                     \n \
                    \\node [nosty] (1) at (3.5, 3 - #1) {};                     \n \
                    \\node [nosty] (2) at (3.5, 3 - #1 - #2) {};                \n \
                    \\node [nosty] (3) at (4.5, 3 - #1 - #2) {};		           \n \
                                                                               \n \
                    \\draw[slit] (1.center) to (0.center);                      \n \
                    \\draw[slit] (1.center) to (2.center);                      \n \
                    \\draw[slit] (2.center) to (3.center);                      \n \
                                                                               \n \
            }                                                                  \n \
                                                                               \n \
            \\newcommand{\\slit}[3]{                                             \n \
                                                                               \n \
                    \\node [nosty] (0) at (-4.5, 3 - #1) {};                    \n \
                    \\node [nosty] (1) at (#3 - 0.25, 3 - #1) {};               \n \
                    \\node [nosty] (2) at (#3 - 0.25, 3 - #1 - #2) {};          \n \
                    \\node [nosty] (3) at (-4.5, 3 - #1 - #2) {};               \n \
                    \\node [nosty] (4) at (-5.0, 3 - #1 - #2 / 2) {$#1$};       \n \
                                                                               \n \
                    \\draw[slit] (1.center) to (0.center);                      \n \
                    \\draw[slit] (1.center) to (2.center);                      \n \
                    \\draw[slit] (2.center) to (3.center);	                   \n \
                                                                               \n \
                    \\node [nosty] (0) at (4.5, 3 - #1) {};                     \n \
                    \\node [nosty] (1) at (#3 + 0.40, 3 - #1) {};               \n \
                    \\node [nosty] (2) at (#3 + 0.40, 3 - #1 - #2) {};          \n \
                    \\node [nosty] (3) at (4.5, 3 - #1 - #2) {};		           \n \
                                                                               \n \
                    \\draw[slit] (1.center) to (0.center);                      \n \
                    \\draw[slit] (1.center) to (2.center);                      \n \
                    \\draw[slit] (2.center) to (3.center);                      \n \
                                                                               \n \
            }                                                                  \n \
                                                                               \n \
            \\newcommand{\\gbar}[5]{                                             \n \
                                                                               \n \
                    \\node [nosty] (0) at (#4,  - #1) {};                      \n \
                    \\node [nosty] (1) at (#3,  - #1) {};                      \n \
                    \\node [nosty] (2) at (#3,  - #1 - #2) {};                \n \
                    \\node [nosty] (3) at (#4,  - #1 - #2) {};                 \n \
                    \\node [nosty] (4) at (#3 * 0.5 + #4 * 0.5 - 0.1, - #1 - #2 * 0.5) {#5}; \n \
                                                                               \n \
                    \\draw[slit] (1.center) to (0.center);                      \n \
                    \\draw[slit] (1.center) to (2.center);                      \n \
                    \\draw[slit] (2.center) to (3.center);                      \n \
                    \\draw[slit] (3.center) to (0.center);	                   \n \
            }                                                                  \n "
    };

    const char* TIKZ_TEX_BOX_STY = {
        "\\tikzstyle{mybox} = [draw=black, fill=white,                  \n \
         rectangle, rounded corners, inner sep=10pt, inner ysep=20pt]  \n \
         \\tikzstyle{fancytitle} =[fill=white, text=black]              \n"
    };

    const char* TIKZ_TEX_BOX_HDR = {
        "\\noindent\\begin{minipage}[c]{10cm}                                \n \
      \\begin{tikzpicture}                                              \n \
        \\node [mybox] (box){                                          \n \
        \\begin{minipage}{0.75\\textwidth}                              \n "
    };

    const char* TIKZ_TEX_BOX_FT = {
        "\\end{minipage}                                                  \n \
     };                                                              \n"
    };

    const char* TIKZ_END_MINI = {
        "\\end{minipage}                                                  \n"
    };

    const char* TIKZ_TEX_BOX_TTL = {
        "\\node[fancytitle, right=10pt] at (box.north west) "
    };


};



}
