# Functors
- Functors are functions on objects.
- Functors map both objects and morphisms.
- Functors preserve connections when it maps morphisms.
- Functors must preserve the structure of a category.
- Collapsing Functor
  - Maps every object in the source category to one selected object in the target category.
  - Maps every morphism to identity morphism.

### Utitlities
```ocaml
let compose f g x = f (g x)
```
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
val f' : ('a -> 'b) -> 'a option -> 'b option = <fun>
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
```ocaml
# let fmap f = function
    | None -> None
    | Some x -> Some (f x)
val fmap : ('a -> 'b) -> 'a option -> 'b option = <fun>
```
- Id example
```ocaml
# let id x = x
val id : 'a -> 'a = <fun>
```
### Utilities needed to compile the code(Can skip this section)
```ocaml
module type Functor = sig 
  type 'a t 
  val fmap : ('a -> 'b) -> 'a t -> 'b t 
end
(* Functor for Option type *)
module OptionF : (Functor with type 'a t = 'a option) = struct 
  type 'a t = 'a option
  let fmap f = function | None -> None | Some x -> Some (f x) 
end 
```
### Test functor laws
- Test Id law (Syntactically correct OCaml but will not be compiled by mdx)
```OCaml
module Test_Functor_Id(F: Functor) = struct 
    open F 
    let test_id x = assert ((fmap id x) = x)
end
```
- Test Compose law (Syntactically correct OCaml but will not be compiled by mdx)
```OCaml
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
module Point_Eq(E:Eq with type a = float) = struct
  type a = point
  let (==) (Pt (p1x, p1y)) (Pt (p2x, p2y)) = E.(p1x == p2x) && E.(p2x == p2y)
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
module Option_Functor:(Functor with type 'a t = 'a option) = struct 
  type 'a t = 'a option 
  let fmap f = function
    | None -> None
    | Some x -> Some (f x)
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
  val fmap : ('a -> 'b) -> 'a list -> 'b list 
end 
```
- Fmap impl for list 
```ocaml
# let rec fmap f = function
    | Nil -> Nil
    | Cons (x, xs) -> Cons (f x, fmap f xs)
val fmap : ('a -> 'b) -> 'a list -> 'b list = <fun>
```
- Functor instance for List 
```ocaml
module List_Functor : (Functor with type 'a t = 'a list) = struct 
  type 'a t = 'a list 
  let rec fmap f = function
    | Nil -> Nil
    | Cons (x, xs) -> Cons (f x, fmap f xs)
end 
```
### Reader Functor
- Function type
```ocaml
type ('a, 'b) t = 'a -> 'b
```
- Partially Applied Function Type
```ocaml
module type T = sig
  type t
end
module Partially_Applied_FunctionType(T : T) = struct
  type 'b t = T.t -> 'b
end
```
- fmap for Reader
```ocaml
module type Reader_Fmap_Example = sig
  val fmap : ('a -> 'b) -> ('r -> 'a) -> 'r -> 'b
end
```
- Functor Instance for Reader
```ocaml
module Reader_Functor(T: T):Functor = struct
  type 'a t = T.t -> 'a
  let fmap f ra = fun r -> f (ra r)
end
```
- Reader Functor implementation - Even simpler
```ocaml
# let fmap: ('a -> 'b) -> ('r -> 'a) -> ('r -> 'b) = compose
val fmap : ('a -> 'b) -> ('r -> 'a) -> 'r -> 'b = <fun>
```

### Functors as Containers
- Infinite list
```ocaml
# let nats = Caml.Stream.from (fun i -> Some (i + 1))
val nats : int Stream.t = <abstr>
```
- Functors can be considered as a container of value(s) of the type over which it is parameterized or as containing a recipe for generating those values.
- It doesn't matter if we are able to access the values inside the functor. All that matters is if we are able to manipulate those values using functions and making sure that these manipulations compose correctly.
- Const
```ocaml
type ('c, 'a) const = Const of 'c
```
- Const fmap signature example
```ocaml
module type Const_Functor_Example = sig
  val fmap : ('a -> 'b) -> ('c, 'a) const -> ('c, 'b) const
end
```
- Const functor instance.
```ocaml
module Const_Functor(T : T) : Functor = struct
  type 'a t = (T.t, 'a) const
  let fmap f (Const c) = Const c (* or even let fmap _ c = c *)
end
```
### Functor Composition
- A composition of two functors when acting on objects is just the composition of their respective object mappings.
- Same for morphisms.
```ocaml
# let maybe_tail = function 
    | [] -> None 
    | _ :: xs -> Some xs
val maybe_tail : 'a list -> 'a list option = <fun>
```
- Using fmap of respective functors
```ocaml
let square x = x * x
let mis = Some (Cons (1, Cons (2, Cons (3, Nil))))
let mis2 = Option_Functor.fmap (List_Functor.fmap square) mis
```
- Composing fmap of list and option functors
```ocaml
let fmapO = Option_Functor.fmap
let fmapL = List_Functor.fmap
let fmapC f l = (compose fmapO fmapL) f l
let mis2 = fmapC (square) mis
```
- Viewing fmap as a function of one argument
```ocaml
module type Fmap_Alt_Sig_Example = sig
  type 'a t
  val fmap : ('a -> 'b) -> ('a t -> 'b t)
end
```
- square signature
```ocaml
module type Square_Signature = sig
  val square : int -> int
end
```
- Return type signature (Syntactically correct Ocaml but not compiled)
```OCaml
int list -> int list
```
- First fmap takes above signature and then returns a function.
```OCaml
int list option -> int list option
```
- Functors form a category.
- Objects are categories. Morphisms are functors.
- *Cat* category of all small categories.
