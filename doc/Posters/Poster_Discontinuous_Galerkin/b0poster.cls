%% 
%% This is file `b0poster.cls'
%%
%% Created by 8th floor CFD group at UNIZAG-FSB based on a0poster class by:
%% 
%% Copyright (C) 1997-2004 Gerlinde Kettl and Matthias Weiser
%%
%% It may be distributed and/or modified under the
%% conditions of the LaTeX Project Public License, either version 1.2
%% of this license or (at your option) any later version.
%% The latest version of this license is in
%%    http://www.latex-project.org/lppl.txt
%% and version 1.2 or later is part of all distributions of LaTeX
%% version 1999/12/01 or later.
%%
%% Problems, bugs and comments to 
%% vuko.vukcevic@fsb.hr
%% 

\ProvidesClass{b0poster}[2017/01/11 b0poster class]
\NeedsTeXFormat{LaTeX2e}[1995/06/01]
\LoadClass{article}

\newif\ifportrait
\newif\ifbzerob
\newif\ifbzero
\newif\ifboneb
\newif\ifbone
\newif\ifbtwo
\newif\ifbthree
\newif\ifposterdraft

\newcount\xkoord
\newcount\ykoord
\newcount\xscale
\newcount\yscale

\DeclareOption{b0b}{% b0 with bleed marks (1cm)
\bzerobtrue\bzerofalse
\xkoord=2594 % big points (1 bp=1/72 inch)
\ykoord=3370 % big points (1 bp=1/72 inch)
\xscale=2
\yscale=2
}
\DeclareOption{b0}{
\bzerotrue\bzerobfalse
\xkoord=2380 % big points (1 bp=1/72 inch)
\ykoord=3368 % big points (1 bp=1/72 inch)
\xscale=2
\yscale=2
}
\DeclareOption{b1b}{% b1 with bleed marks (1cm)
\bonebtrue\bonefalse\bzerobfalse
\xkoord=1898 % big points (1 bp=1/72 inch)
\ykoord=2382 % big points (1 bp=1/72 inch)
\xscale=3
\yscale=3
}
\DeclareOption{b1}{
\bonetrue\bonebfalse\bzerobfalse
\xkoord=1684 % big points (1 bp=1/72 inch)
\ykoord=2380 % big points (1 bp=1/72 inch)
\xscale=3
\yscale=3
}
\DeclareOption{b2}{
\btwotrue\bonebfalse\bzerobfalse
\xkoord=1190 % big points (1 bp=1/72 inch)
\ykoord=1684 % big points (1 bp=1/72 inch)
\xscale=4
\yscale=4

}
\DeclareOption{b3}{
\bthreetrue\bonebfalse\bzerobfalse
\xkoord=842  % big points (1 bp=1/72 inch)
\ykoord=1190 % big points (1 bp=1/72 inch)
\xscale=6
\yscale=6
}
\DeclareOption{landscape}{
\portraitfalse
}
\DeclareOption{portrait}{
\portraittrue
}
\DeclareOption{draft}{ %% Just for downward compatibility
\posterdrafttrue
}
\DeclareOption{posterdraft}{ %% To avoid collisions with the graphics package
\posterdrafttrue
}
\DeclareOption{final}{
\posterdraftfalse
}
\DeclareOption*{\PackageWarning{b0poster}{Unknown Option \CurrentOption}}
\ExecuteOptions{landscape,b0b,final}
\ProcessOptions\relax

\ifbzerob
   \setlength{\paperwidth}{143.4cm}
   \setlength{\paperheight}{102cm}
   \setlength{\textwidth}{138.4cm}
   \setlength{\textheight}{97cm}
\else\ifbzero
        \setlength{\paperwidth}{141.4cm}
        \setlength{\paperheight}{100cm}
        \setlength{\textwidth}{138.4cm}
        \setlength{\textheight}{97cm}
     \else\ifboneb
             \setlength{\paperwidth}{102cm}
             \setlength{\paperheight}{72.7cm}
             \setlength{\textwidth}{97cm}
             \setlength{\textheight}{67.7cm}
          \else\ifbone
                  \setlength{\paperwidth}{100cm}
                  \setlength{\paperheight}{70.7cm}
                  \setlength{\textwidth}{97cm}
                  \setlength{\textheight}{67.7cm}
               \else\ifbtwo
                       \setlength{\paperwidth}{70.7cm}
                       \setlength{\paperheight}{50cm}
                       \setlength{\textwidth}{67.7cm}
                       \setlength{\textheight}{47cm}
                   \else\ifbtwo
                           \setlength{\paperwidth}{50cm}
                           \setlength{\paperheight}{35.3cm}
                           \setlength{\textwidth}{47cm}
                           \setlength{\textheight}{32.3cm}
                        \else\relax
                        \fi
                    \fi
               \fi
          \fi
     \fi
\fi


\ifportrait
   \newdimen\tausch
   \setlength{\tausch}{\paperwidth}
   \setlength{\paperwidth}{\paperheight}
   \setlength{\paperheight}{\tausch}
   \setlength{\tausch}{\textwidth}
   \setlength{\textwidth}{\textheight}
   \setlength{\textheight}{\tausch}
\else\relax
\fi

%% Setting proper dimensions for a DIN A0 printer

\setlength{\headheight}{0 cm}
\setlength{\headsep}{0 cm}
\setlength{\topmargin}{0 cm}
\setlength{\oddsidemargin}{0 cm}

\catcode`\%=11
\newwrite\Ausgabe
\immediate\openout\Ausgabe=a0header.ps
\write\Ausgabe{%%BeginFeature *PageSize ISOA0/ISO A0}
\ifposterdraft\write\Ausgabe{2 dict dup /PageSize [595 842] put dup /ImagingBBox null put}
\else\write\Ausgabe{2 dict dup /PageSize [\number\xkoord\space \number\ykoord] put dup /ImagingBBox null put}\fi
\write\Ausgabe{setpagedevice}
\ifposterdraft\write\Ausgabe{38 84 translate}
\write\Ausgabe{0.\number\xscale\space 0.\number\yscale\space scale}\else\relax\fi
\write\Ausgabe{%%EndFeature}
\closeout\Ausgabe
\catcode`\%=14

\special{header=./a0header.ps}

\input{a0size.sty}

\renewcommand{\tiny}{\fontsize{12}{14}\selectfont}
\renewcommand{\scriptsize}{\fontsize{14.4}{18}\selectfont}   
\renewcommand{\footnotesize}{\fontsize{17.28}{22}\selectfont}
\renewcommand{\small}{\fontsize{20.74}{25}\selectfont}
\renewcommand{\normalsize}{\fontsize{24.88}{30}\selectfont}
\renewcommand{\large}{\fontsize{29.86}{37}\selectfont}
\renewcommand{\Large}{\fontsize{35.83}{45}\selectfont}
\renewcommand{\LARGE}{\fontsize{43}{54}\selectfont}
\renewcommand{\huge}{\fontsize{51.6}{64}\selectfont}
\renewcommand{\Huge}{\fontsize{61.92}{77}\selectfont}
\newcommand{\veryHuge}{\fontsize{74.3}{93}\selectfont}
\newcommand{\VeryHuge}{\fontsize{89.16}{112}\selectfont}
\newcommand{\VERYHuge}{\fontsize{107}{134}\selectfont}

\setlength\smallskipamount{6pt plus 2pt minus 2pt}
\setlength\medskipamount{12pt plus 4pt minus 4pt}
\setlength\bigskipamount{24pt plus 8pt minus 8pt}

\setlength\abovecaptionskip{25pt}
\setlength\belowcaptionskip{0pt}
\setlength\abovedisplayskip{25pt plus 6pt minus 15 pt}
\setlength\abovedisplayshortskip{0pt plus 6pt}
\setlength\belowdisplayshortskip{13pt plus 7pt minus 6pt}
\setlength\belowdisplayskip\abovedisplayskip

\normalsize

\AtBeginDocument{
% Helper commands for margins
\newcommand\done{1.666666}
\newcommand\dtwo{0.833333}
\ifbzerob
   \ifportrait\special{papersize=102cm,143.4cm}\else\special{papersize=143.4cm,102cm}\fi
    %---------------------------------------------------------------------------
    %	BLEED MARKS (bleed marks = 1cm)
    %---------------------------------------------------------------------------
    \begin{tikzpicture}[overlay, remember picture]
        \draw ($(current page text area.north west)+(-\done,\done)$) -- ++  (-\dtwo,0);
        \draw ($(current page text area.north west)+(-\done,\done)$) -- ++  (0,\dtwo);
    
        \draw ($(current page text area.north east)+(\done,\done)$) -- ++  (\dtwo,0);
        \draw ($(current page text area.north east)+(\done,\done)$) -- ++  (0,\dtwo);

        \draw ($(current page text area.south west)+(-\done,-\done)$) -- ++  (-\dtwo,0);
        \draw ($(current page text area.south west)+(-\done,-\done)$) -- ++  (0,-\dtwo);

        \draw ($(current page text area.south east)+(\done,-\done)$) -- ++  (\dtwo,0);
        \draw ($(current page text area.south east)+(\done,-\done)$) -- ++  (0,-\dtwo);
    \end{tikzpicture}
\else\ifbzero
        \ifportrait\special{papersize=100cm,141.4cm}\else\special{papersize=141.4cm,100cm}\fi
     \else\ifboneb
             \ifportrait\special{papersize=72.7cm,102cm}\else\special{papersize=102cm,72.7cm}\fi
             %------------------------------------------------------------------
             %	BLEED MARKS (bleed marks = 1cm)
             %------------------------------------------------------------------
             \begin{tikzpicture}[overlay, remember picture]
                 \draw ($(current page text area.north west)+(-\done,\done)$) -- ++  (-\dtwo,0);
                 \draw ($(current page text area.north west)+(-\done,\done)$) -- ++  (0,\dtwo);
             
                 \draw ($(current page text area.north east)+(\done,\done)$) -- ++  (\dtwo,0);
                 \draw ($(current page text area.north east)+(\done,\done)$) -- ++  (0,\dtwo);

                 \draw ($(current page text area.south west)+(-\done,-\done)$) -- ++  (-\dtwo,0);
                 \draw ($(current page text area.south west)+(-\done,-\done)$) -- ++  (0,-\dtwo);

                 \draw ($(current page text area.south east)+(\done,-\done)$) -- ++  (\dtwo,0);
                 \draw ($(current page text area.south east)+(\done,-\done)$) -- ++  (0,-\dtwo);
             \end{tikzpicture}
          \else\ifbone
                  \ifportrait\special{papersize=70.7cm,100cm}\else\special{papersize=100cm,70.7cm}\fi
               \else\ifbtwo
                      \ifposterdraft
                       \ifportrait\special{papersize=49cm,70cm}\else\special{papersize=70cm,49cm}\fi
                      \else
                       \ifportrait\special{papersize=50cm,70.7cm}\else\special{papersize=70.7cm,50cm}\fi
                      \fi
                    \else\ifbthree
                           \ifposterdraft
                            \ifportrait\special{papersize=34cm,49cm}\else\special{papersize=49cm,34cm}\fi
                           \else
                            \ifportrait\special{papersize=35.3cm,50cm}\else\special{papersize=50cm,35.3cm}\fi
                           \fi
                         \else\relax
                         \fi
                    \fi
               \fi
          \fi
     \fi
\fi
\pagestyle{empty}
}

\endinput
%% 
%% End of file `b0poster.cls'.
