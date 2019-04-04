# Chapter 1 Code Snippets
* A generic function from type a to type b.
```ocaml
module type Polymorphic_Function = sig
  type a
  type b
  val f : a -> b
end
```
* Compose Function definition is not part of the standard library. We can define a custom /compose/ function.
```ocaml
# let (>>) f g x = g (f x)
val ( >> ) : ('a -> 'b) -> ('b -> 'c) -> 'a -> 'c = <fun>
```
* Compose is a polymorphic function. So, you can call it with any two function as long as the output of one matches the input of the other.
```ocaml
module StringToInt : (Polymorphic_Function with type a = string and type b = int) = struct
  type a = string
  type b = int
  let f = String.length
end

module IntToBool : (Polymorphic_Function with type a = int and type b = bool) = struct
  type a = int
  type b = bool
  let f a = a > 1
end

module BoolToInt : (Polymorphic_Function with type a = bool and type b = int) = struct
  type a = bool
  type b = int
  let f a = match a with
    | false -> 0
    | true -> 1
end
```
```ocaml
# let f = StringToInt.f
val f : string -> int = <fun>
# let g = IntToBool.f
val g : int -> bool = <fun>
```
* Now Call Compose
```ocaml
# f >> g
- : string -> bool = <fun>
```

* Properties of Composition
```ocaml
# let f = StringToInt.f
val f : string -> int = <fun>
# let g = IntToBool.f
val g : int -> bool = <fun>
# let h = BoolToInt.f
val h : bool -> int = <fun>
```
* Pseudo OCaml showing equivalence relation among functions
```pseudo-ocaml
f >> (g >> h) == (f >> g) >> h == f >> g >> h
```
* Identity Function
```ocaml
# let id x = x
val id : 'a -> 'a = <fun>
```
* Compose and Identity
```ocaml
# f >> id
- : string -> int = <fun>
# id >> f
- : string -> int = <fun>
```
