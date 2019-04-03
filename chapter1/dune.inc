(alias
 (name   runtest)
 (deps   (:x chapter1.md))
 (action (progn
           (run ocaml-mdx test --direction=to-ml %{x})
           (diff? %{x} %{x}.corrected)
)))
