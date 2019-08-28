# Declarative Programming
### Utilities (Needed for compiling the code. Skip this section)
```ocaml
let compose g f x = g (f x)
```
- Snippets marked as Pseudo OCaml are not compiled by mdx
### Declarative Programming in compose
- Compose (declarative)
```OCaml
(* Assume g and f are already defined *)
let h = compose g f
```
- Compose (imperative)
```OCaml
let h = fun x -> 
  let y = f x in
  g y
```
- Compose (pipe)
```OCaml
let h = fun x -> x |> f |> g
```
