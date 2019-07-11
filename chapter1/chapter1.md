# Chapter 1 - The essence of composition
### Utilities (Feel free to skip. Used to compile code below)
```ocaml
# let (>>) : 'a 'b 'c. ('b -> 'c) -> ('a -> 'b) -> 'a -> 'c = 
    fun g f x -> g (f x)
val ( >> ) : ('b -> 'c) -> ('a -> 'b) -> 'a -> 'c = <fun>
# let f : string -> int = String.length
val f : string -> int = <fun>
```
### Code snippets
* A generic function from type a to type b.
```ocaml
module type Polymorphic_Function_F = sig
  type a
  type b
  val f : a -> b
end
```
* Another polymorphic function from type b to type c.
```ocaml
module type Polymorphic_Function_G = sig
  type b
  type c
  val g : b -> c
end
```
* Compose Function definition is not part of the standard library. We can define a custom /compose/ function.
```ocaml
module Compose_Example = functor(F: Polymorphic_Function_F)(G: Polymorphic_Function_G with type b = F.b) -> struct
   (** OCaml doesn't have a compose operator. So, creating one. **)
   let (>>) g f x = g (f x)
   let compose : 'a -> 'c = G.g >> F.f
end
```
* Compose Three functions
```OCaml
module Compose_Three_GF = functor(F:Polymorphic_Function_F)(G:Polymorphic_Function_G with type b = F.b)(H:Polymorphic_Function_H with type c = G.c) -> struct
  let compose : 'a -> 'd = H.h >> (G.g >> F.f)
end

module Compose_Three_HG = functor(F:Polymorphic_Function_F)(G:Polymorphic_Function_G with type b = F.b)(H:Polymorphic_Function_H with type c = G.c) -> struct
  let compose : 'a -> 'd = (H.h >> G.g) >> F.f
end

Compose_Three_GF.compose = Compose_Three_HG.compose
```
* Identity Function
```ocaml
# let id x = x
val id : 'a -> 'a = <fun>
```
* Compose and Identity
```OCaml
f >> id
id >> f
```
