# Functors
- Functors are functions on objects.
- Functors map both objects and morphisms.
- Functors preserve connections when it maps morphisms.
- Functors must preserve the structure of a category.
- Collapsing Functor
  - Maps every object in the source category to one selected object in the target category.
  - Maps every morphism to identity morphism.
  
### Maybe Functor
- Maybe functor
```ocaml
type 'a option = None | Some of 'a
```
- Example function
```ocaml
module type AtoB = sig
  type a
  type b
  val f : a -> b
end
```
- Function from 'a option to 'b option
```ocaml
# let f' f = function | None -> None | Some x -> Some (f x)
```
- Morphism mapping as `fmap`
```ocaml
module type Maybe_Functor = sig
  type a
  type b
  val fmap : (a -> b) -> (a option -> b option)
end
```
- fmap as a function with two arguments.
```ocaml
module type Maybe_Functor = sig
  type a
  type b
  val fmap : (a -> b) -> a option -> b option
end
```
- Maybe Functor Example implementation.
```
# let fmap f = function | None -> None | Some x -> Some (f x)
```
- Id example
```ocaml
# let id x = x
val id : 'a -> 'a = <fun>
```
- Functor Representation in OCaml
```ocaml
module type Functor = sig 
  type 'a t 
  val fmap : ('a -> 'b) -> 'a t -> 'b t 
end 
``` 
- Functor for Option type 
```ocaml 
module OptionF : (Functor with type 'a t = 'a option) = struct 
  type 'a t = 'a option
  let fmap f = function | None -> None | Some x -> Some (f x) 
end 
``` 
- Test Functor laws. 
- Test Id law 
```ocaml 
module Test_Functor_Id(F: Functor) = struct 
  open F 
  let test_id x = assert ((fmap id x) = x)
end 
``` 
- Test Compose law 
```ocaml 
module Test_Functor_Compose(F: Functor) = struct 
  open F
  
  (* Compose *)
  let <.> f g x = f (g x) 
  
  let test_compose f g x = assert (fmap (f <.> g) x = fmap f (fmap g x))
end 
``` 
### Typeclasses 
- OCaml doesn't have typeclasses. 
- But it has functor modules. 
```ocaml 
module type Eq = sig 
  type a 
  val (==) : a -> a -> bool 
end 
``` 
- Point data type 
```ocaml 
type point = Pt of float * float 
``` 
- Eq instance for Point 
```ocaml 
module Float_Eq = struct 
  type float 
  let (==) x y = x = y 
end 
``` 
- Functor for OCaml 
```ocaml 
module type Functor = sig 
  type 'a t 
  val fmap : ('a -> 'b) -> 'a t -> 'b t 
end 
``` 
- Functor instance for Option 
```ocaml 
module Option_Functor = struct 
  type 'a t = 'a option 
  let fmap f = function | None -> None | Some x -> Some (f x) 
end 
``` 
- List Functor 
```ocaml 
type 'a list = Nil | Cons of 'a * 'a list 
``` 
- Fmap for list 
```ocaml 
module type List_Functor_Type = sig  
  type 'a t = 'a list  
  val fmap : ('a -> 'b) -> 'a list -> 'a list 
end 
``` 
- Fmap impl for list 
```ocaml 
# let rec fmap f = function | Nil -> Nil | Cons (x, xs) -> Cons (f x, fmap f xs) 
val fmap : ('a -> 'b) -> 'a list -> 'b list = <fun> 
``` 
- Functor instance for List 
```ocaml 
module List_Functor = struct 
  type 'a t = 'a list 
  let rec fmap f = function | Nil -> Nil | Cons (x, xs) -> Cons (f x, fmap f xs) 
end 
```
