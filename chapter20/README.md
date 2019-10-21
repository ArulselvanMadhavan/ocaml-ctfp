# Monads
## Utilities used by code below
```ocaml
let ( <.> ) f g x = f (g x)
let flip f x y = f y x
type ('w, 'a) writer = Writer of ('a * 'w)
module type Functor = sig
  type 'a t
  val fmap : ('a -> 'b) -> 'a t -> 'b t
end
module type Monoid = sig
  type a
  val mempty : a
  val mappend : a -> a -> a
end
let up_case = fun s -> Writer(String.uppercase_ascii s, "up_case ")
```
## Introduction
```ocaml
(* Depends on OCaml library Base - #require "base" *)
module Vlen(F : Functor with type 'a t = 'a list) = struct
  
  open Base
  
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
module WriterMonad(W : Monoid): Monad with type 'a m = (W.a, 'a) writer = struct
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
- Step 1
```OCaml
let (>=>) f g = fun a -> ...
```
- Step 2
```OCaml
let (>=>) f g = fun a -> 
  let mb = f a in
  ...
```
- Step 3
```OCaml
val (>>=) : 'a m -> ('a -> 'b m) -> 'b m
```
- Monad using bind
```ocaml
module type Monad_Bind =
  sig
    type 'a m
    val ( >>= ) : 'a m -> ('a -> 'b m) -> 'b m
    val return : 'a -> 'a m
  end
```
- Writer using bind
```ocaml
module WriterMonadBind(W : Monoid) = struct
  let (>>=) (Writer (a, w)) f = 
    let Writer (b, w') = f a in
    Writer (b, W.mappend w w')
end
```
- join in ocaml
```OCaml
val join : ('a m) m -> 'a m
```
- Rewrite bind as
```ocaml
module BindUsingFunctionAndJoin(F : Functor) = struct
  type 'a m = 'a F.t
  
  (** Make the type signature of join work 
  without providing an implementation. **)
  external join : 'a m m -> 'a m = "%identity"
  
  let (>>=) ma f = join (F.fmap f ma)
end
```
- Third option for defining a Monad
```ocaml
module type Monad_Join = functor (F : Functor) -> sig
  type 'a m = 'a F.t
  val join : 'a m m -> 'a m
  val return : 'a -> 'a m
end
```
- fmap in terms of bind and return
```ocaml
module Fmap_Using_Monad(M : Monad_Bind) = struct
  let fmap f ma = M.(>>=) ma (fun a -> M.return (f a))
end
```
- join for the Writer monad
```ocaml
module Writer_Join(W : Monoid) = struct
  let join (Writer (Writer (a, w'), w)) = Writer (a, W.mappend w w')
end
```
## do Notation
- Writer example
```ocaml
(* Using split_on_char to avoid having to use external regex libraries *)
let to_words = fun s -> Writer (String.split_on_char ' ' s, "to_words")

module Writer_Process(W : Monad with type 'a m = (string, 'a) writer) =
struct
  let process = W.(up_case >=> to_words)
end
```
- OCaml do Notation
```ocaml
module Process_Do(W : Monad_Bind with type 'a m = (string, 'a) writer) = 
struct

  (* Needs OCaml compiler >= 4.08 *)
  let (let*) = W.(>>=)
  
  let process s = 
    let* up_str = up_case s in
    to_words up_str
end
```
- Upcase 
```ocaml
let up_case = fun s -> Writer(String.uppercase s, "up_case ")
```
- Do block is 
```ocaml
module Process_Bind_Without_Do(W : Monad_Bind with type 'a m = (string, 'a) writer) = 
struct
  let process s = W.(up_case s >>= (fun up_str -> to_words up_str))
end
```
- Up Case
```OCaml
let* up_str <- up_case s
```
- Using teller towords
```ocaml
module Process_Tell(W : Monad_Bind with type 'a m = (string, 'a) writer) = 
struct
  (* Needs OCaml compiler >= 4.08 *)
  let (let*) = W.(>>=)

  let tell w = Writer ((), w)

  let process s = 
    let* up_str = up_case s in
    let* _ = tell "to_words " in
    to_words up_str
end;;
```
- With bind
```ocaml
module Process_Bind_Without_Do(W : Monad_Bind with type 'a m = (string, 'a) writer) = 
struct
  let tell w = Writer((), w)  
  let process s = W.(up_case s >>= (fun up_str -> 
      tell "to_words" >>= (fun _ -> 
      to_words up_str)))
end;;
```
- Special operator (>>)
```ocaml
module Monad_Ops(M : Monad_Bind) = struct
  let (>>) m k = M.(m >>= (fun _ -> k))
end
```
- Process with special ops
```ocaml
module Process_Bind_Without_Do(W : Monad_Bind with type 'a m = (string, 'a) writer) = 
struct
  open Monad_Ops(W)
  let tell w = Writer((), w)  
  let process s = W.(up_case s >>= (fun up_str -> 
      tell "to_words" >> 
      to_words up_str))
end
```
