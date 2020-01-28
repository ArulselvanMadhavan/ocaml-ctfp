# Lawvere Theories
```ocaml
type ('a, 'b) either = Left of 'a | Right of 'b
type two = (unit, unit) either
```
```OCaml
val raise : unit -> 'a
```
```ocaml
type 'a option = (unit, 'a) either
```
```ocaml
type 'a option = None | Some of 'a
```
