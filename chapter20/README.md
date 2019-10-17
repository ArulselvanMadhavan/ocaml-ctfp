# Monads
## Utilities used by code below
```ocaml
let ( <.> ) f g x = f (g x)
let flip f x y = f y x
module type Functor = sig
  type 'a t
  val fmap : ('a -> 'b) -> 'a t -> 'b t
end
module type Monoid = sig
  type a
  val mempty : a
  val mappend : a -> a -> a
end
```
## Introduction
```ocaml
(* Depends on OCaml library Core *)
module Vlen(F : Functor with type 'a t = 'a list) = struct
  
  let summable = (module Float:Base__.Container_intf.Summable with type t = float)
  
  let vlen = Float.sqrt <.> (List.sum summable ~f:Fn.id) <.> (F.fmap (flip Float.int_pow 2))
end
```
## Kleisli Category
- Writer's Functor Instance
```ocaml
module WriterInstance (W : sig type w end) : Functor with type 'a t = (W.w, 'a) writer = struct
  type 'a t = (W.w, 'a) writer
  let fmap f (Writer (a, w)) = Writer (f a, w)
end
```
- Composing embellished functions
```OCaml
'a -> ('w, 'b) writer
```
- Kleisli Category K has the same objects as C but its morphisms are different.
- A morphism between a and b in K is implemented as a morphism a -> m b in C
- Functor m that has a composition, which is associative, and has an identity arrow for every object, is called a monad
```ocaml
module type Monad =
  sig
    type 'a m
    val ( >=> ) : ('a -> 'b m) -> ('b -> 'c m) -> 'a -> 'c m
    val return : 'a -> 'a m
  end
```
- There are other ways of defining a monad
- Monad is a way of composing embellished functions
- Monad Instance for Writer
```ocaml
module WriterMonad(W : Monoid):Monad with type 'a m = (W.a, 'a) writer = struct
  type 'a m = (W.a, 'a) writer

  let (>=>) f g = fun a ->
    let Writer (b, w) = f a in
    let Writer (c, w') = g b in
    Writer (c, W.mappend w w')
 
  let return a = Writer (a, W.mempty)
end
```
- Tell for Writer
```ocaml
let tell w = Writer ((), w)
```
## Fish Anatomy
