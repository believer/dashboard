open Dom.Storage;

type p;

[@bs.module "push.js"] [@bs.val]
external createWithOptions: (string, Js.t('a)) => unit = "create";

let createNotification = (~title, ~body="", ()) =>
  createWithOptions(
    title,
    {"body": body, "icon": "/logo.png", "timeout": 3000},
  );

let sendGitHubNotification = notifications => {
  let githubNotified = "github_notified";
  let seen =
    (
      switch (localStorage |> getItem(githubNotified)) {
      | Some(value) => value
      | None => ""
      }
    )
    |> Js.String.split(",");

  let unseen =
    notifications
    |> Js.Array.filter(item => !Js.Array.includes(item##id, seen));

  let ids = unseen |> Js.Array.map(item => item##id);

  switch (Array.length(unseen)) {
  | 0 => ()
  | 1 =>
    createNotification(
      ~title=unseen[0]##subject##title,
      ~body=unseen[0]##repository##name,
      (),
    );

    localStorage
    |> setItem(
         githubNotified,
         Js.Array.concat(seen, ids) |> Js.Array.joinWith(","),
       );
  | v =>
    createNotification(
      ~title=(v |> string_of_int) ++ " new notifications",
      (),
    );

    localStorage
    |> setItem(
         githubNotified,
         Js.Array.concat(seen, ids) |> Js.Array.joinWith(","),
       );
  };
};

let sendTrelloNotification = notifications => {
  let trelloNotified = "trello_notified";
  let seen =
    (
      switch (localStorage |> getItem(trelloNotified)) {
      | Some(value) => value
      | None => ""
      }
    )
    |> Js.String.split(",");

  let unseen =
    notifications
    |> Js.Array.filter(item => !Js.Array.includes(item##id, seen));

  let ids = unseen |> Js.Array.map(item => item##id);

  switch (Array.length(unseen)) {
  | 0 => ()
  | 1 =>
    let title =
      switch (unseen[0]##data##card) {
      | Some(c) => c##name
      | None => "1 new notification"
      };

    let body =
      switch (unseen[0]##data##board) {
      | Some(b) => b##name
      | None => ""
      };

    createNotification(~title, ~body, ());

    localStorage
    |> setItem(
         trelloNotified,
         Js.Array.concat(seen, ids) |> Js.Array.joinWith(","),
       );
  | v =>
    createNotification(
      ~title=(v |> string_of_int) ++ " new notifications",
      (),
    );
    localStorage
    |> setItem(
         trelloNotified,
         Js.Array.concat(seen, ids) |> Js.Array.joinWith(","),
       );
  };
};
