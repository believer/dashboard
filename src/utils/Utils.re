let str = ReasonReact.string;

let groupTrelloByDate = (arr: array(Trello.notification)) => {
  let dict = Js.Dict.empty();

  arr
  |> Js.Array.reduce(
       (acc, curr: Trello.notification) => {
         let date = DateFns.format(curr.date, "dddd MMM DD");

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

let groupGitHubByDate = (arr: array(GitHub.notification)) => {
  let dict = Js.Dict.empty();

  arr
  |> Js.Array.reduce(
       (acc, curr: GitHub.notification) => {
         let date = DateFns.format(curr.date, "dddd MMM DD");

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
