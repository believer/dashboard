open Dom.Storage;

type p;

[@bs.module "push.js"] [@bs.val] external create: string => unit = "create";

let sendGitHubNotification = (notifications: array(GitHub.notification)) => {
  let githubNotified = "github_notified";
  let seen =
    (
      switch (localStorage |> getItem(githubNotified)) {
      | Some(value) => value
      | None => ""
      }
    )
    |> Js.String.split(",");

  let unseen: array(GitHub.notification) =
    notifications
    |> Js.Array.filter((item: GitHub.notification) =>
         !Js.Array.includes(item.id, seen)
       );

  let ids = unseen |> Js.Array.map((item: GitHub.notification) => item.id);

  switch (Array.length(unseen)) {
  | 0 => ()
  | 1 => create(unseen[0].subject.title)
  | v => create((v |> string_of_int) ++ " new notifications")
  };

  switch (Array.length(unseen)) {
  | 0 => ()
  | 1 =>
    create(unseen[0].subject.title);

    localStorage
    |> setItem(
         githubNotified,
         Js.Array.concat(seen, ids) |> Js.Array.joinWith(","),
       );
  | v =>
    create((v |> string_of_int) ++ " new notifications");
    localStorage
    |> setItem(
         githubNotified,
         Js.Array.concat(seen, ids) |> Js.Array.joinWith(","),
       );
  };
};

let sendTrelloNotification = (notifications: array(Trello.notification)) => {
  let trelloNotified = "trello_notified";
  let seen =
    (
      switch (localStorage |> getItem(trelloNotified)) {
      | Some(value) => value
      | None => ""
      }
    )
    |> Js.String.split(",");

  let unseen: array(Trello.notification) =
    notifications
    |> Js.Array.filter((item: Trello.notification) =>
         !Js.Array.includes(item.id, seen)
       );

  let ids = unseen |> Js.Array.map((item: Trello.notification) => item.id);

  switch (Array.length(unseen)) {
  | 0 => ()
  | 1 =>
    (
      switch (unseen[0].data.card) {
      | Some(c) => c.name
      | None => "1 new notification"
      }
    )
    |> create;
    localStorage
    |> setItem(
         trelloNotified,
         Js.Array.concat(seen, ids) |> Js.Array.joinWith(","),
       );
  | v =>
    create((v |> string_of_int) ++ " new notifications");
    localStorage
    |> setItem(
         trelloNotified,
         Js.Array.concat(seen, ids) |> Js.Array.joinWith(","),
       );
  };
};
