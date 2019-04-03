# Chapter 1 Code Snippets
* A generic function from type a to type b.
```ocaml
module type Chapter1_Snippet1 = sig
  type a
  type b
  val f : a -> b
end
```
* A generic function from type b to type a.
```ocaml
module type Chapter1_Snippet2 = sig
  type b
  type c
  val g : b -> c
end
```
* Compose Function definition is not part of the standard library. We can define a custom /compose/ function.
```ocaml
# let compose f g x = g (f x)
val compose : ('a -> 'b) -> ('b -> 'c) -> 'a -> 'c = <fun>
```
* Compose is a polymorphic function. So, you can call it with any two function as long as the output of one matches the input of the other.
```ocaml
module StringToInt: (Chapter1_Snippet1 with type a = string and type b = int) = struct
  type a = string
  type b = int
  let f s = String.length s
end

module IntToBool:(Chapter1_Snippet2 with type b = int and type c = bool) = struct 
  type b = int
  type c = bool
  let g i = i <= 7
end
```
* Now Call Compose
```ocaml
# compose StringToInt.f IntToBool.g
- : string -> bool = <fun>
```

