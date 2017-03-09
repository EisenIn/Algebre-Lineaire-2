(TeX-add-style-hook
 "notes"
 (lambda ()
   (TeX-add-to-alist 'LaTeX-provided-class-options
                     '(("scrbook" "a4paper" "11pt" "french")))
   (TeX-add-to-alist 'LaTeX-provided-package-options
                     '(("inputenc" "utf8") ("babel" "french") ("hyperref" "colorlinks")))
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "path")
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "url")
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "nolinkurl")
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "hyperbaseurl")
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "hyperimage")
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "hyperref")
   (add-to-list 'LaTeX-verbatim-macros-with-delims-local "path")
   (add-to-list 'LaTeX-verbatim-macros-with-delims-local "url")
   (TeX-run-style-hooks
    "latex2e"
    "chapter01-produits-scalaires"
    "chapter02-theoreme-spectral"
    "chapter03-systemes-differentiels-lineaires"
    "scrbook"
    "scrbook11"
    "inputenc"
    "utf8math"
    "mathrsfs"
    "babel"
    "amsmath"
    "amssymb"
    "amsthm"
    "amscd"
    "todonotes"
    "multirow"
    "enumerate"
    "tikz"
    "framed"
    "hyperref")
   (TeX-add-symbols
    '("wt" 1)
    '("pscal" 1)
    "E"
    "N"
    "Q"
    "R"
    "Z"
    "C"
    "K"
    "x"
    "y"
    "X"
    "cA"
    "cD"
    "cI"
    "cP"
    "cV"
    "car"
    "one")
   (LaTeX-add-environments
    "theorem"
    "lemma"
    "proposition"
    "claim"
    "corollary"
    "definition"
    "notation"
    "example"
    "remark"
    "problem"
    "exercise"
    "algorithm")
   (LaTeX-add-bibliographies
    "books")))

