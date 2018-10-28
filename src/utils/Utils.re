let str = ReasonReact.string;

let groupByDate = arr => {
  let dict = Js.Dict.empty();

  arr
  |> Js.Array.reduce(
       (acc, curr) => {
         let date = DateFns.format(curr##date, "dddd MMM DD");

         switch (Js.Dict.get(dict, date)) {
         | None => Js.Dict.set(dict, date, [|curr|])
         | Some(oldValue) =>
           Js.Dict.set(dict, date, oldValue |> Js.Array.concat([|curr|]))
         };

         acc;
       },
       [||],
     )
  |> ignore;

  dict;
};
