type notificationType =
  | PullRequest
  | Issue
  | UnknownNotification;

type notificationReason =
  | Subscribed
  | ReviewRequested
  | UnknownReason;

type notificationSubject = {
  title: string,
  url: string,
  type_: notificationType,
  latestCommentUrl: string,
};

type notification = {
  id: string,
  unread: bool,
  subject: notificationSubject,
  reason: notificationReason,
  updatedAt: string,
};

module Decode = {
  open Json.Decode;

  let notificationSubject = json => {
    title: json |> field("title", string),
    url: json |> field("url", string),
    type_:
      switch (json |> field("type", string)) {
      | "PullRequest" => PullRequest
      | "Issue" => Issue
      | _ => UnknownNotification
      },
    latestCommentUrl: json |> field("latest_comment_url", string),
  };

  let parseNotifications = json => {
    id: json |> field("id", string),
    unread: json |> field("unread", bool),
    subject: json |> field("subject", notificationSubject),
    updatedAt: json |> field("updated_at", string),
    reason:
      switch (json |> field("reason", string)) {
      | "review_requested" => ReviewRequested
      | "subscribed" => Subscribed
      | _ => UnknownReason
      },
  };

  let decodeNotifications = json => json |> array(parseNotifications);
};

let getNotifications = () => {
  let request =
    Axios.makeConfigWithUrl(
      ~url="https://api.github.com/notifications",
      ~_method="GET",
      ~headers={"Authorization": "Token " ++ Storage.getConfig("github")},
      (),
    );

  Js.Promise.(
    Axios.request(request)
    |> then_(response =>
         response##data |> Decode.decodeNotifications |> resolve
       )
  );
};
