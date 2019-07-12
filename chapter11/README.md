# Declarative Programming
### Utilities (Needed for compiling the code. Skip this section)
```ocaml
let compose g f x = g (f x)
```
- Snippets marked as Pseudo OCaml are not compiled by mdx
### Declarative Programming in compose
- Compose(declarative) - Pseudo OCaml
```OCaml
(* Assume g and f are already defined *)
let h = compose g f
```
- Compose(Imperative) - Pseudo OCaml
```OCaml
let h = fun x -> 
  let y = f x in
  g y
```
