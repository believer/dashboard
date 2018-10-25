type notificationType =
  | Commit
  | PullRequest
  | Issue
  | VulnerabilityAlert
  | UnknownNotification;

type notificationReason =
  | Subscribed
  | ReviewRequested
  | UnknownReason;

type notificationSubject = {
  title: string,
  url: string,
  type_: notificationType,
  latestCommentUrl: option(string),
};

type gitRepo = {name: string};

type notification = {
  id: string,
  unread: bool,
  subject: notificationSubject,
  reason: notificationReason,
  repository: gitRepo,
  updatedAt: string,
};

module Decode = {
  open Json.Decode;

  let notificationSubject = json => {
    title: json |> field("title", string),
    url: json |> field("url", string),
    type_:
      switch (json |> field("type", string)) {
      | "Commit" => Commit
      | "PullRequest" => PullRequest
      | "Issue" => Issue
      | "RepositoryVulnerabilityAlert" => VulnerabilityAlert
      | _ => UnknownNotification
      },
    latestCommentUrl: json |> optional(field("latest_comment_url", string)),
  };

  let gitRepo = json => {name: json |> field("full_name", string)};

  let parseNotifications = json => {
    id: json |> field("id", string),
    unread: json |> field("unread", bool),
    subject: json |> field("subject", notificationSubject),
    updatedAt: json |> field("updated_at", string),
    repository: json |> field("repository", gitRepo),
    reason:
      switch (json |> field("reason", string)) {
      | "review_requested" => ReviewRequested
      | "subscribed" => Subscribed
      | _ => UnknownReason
      },
  };

  let decodeNotifications = json => json |> array(parseNotifications);
};

module Config = {
  open Dom.Storage;

  let notificationConfig = "github_notifications";

  let hasConfig = Storage.hasConfig("github");
  let numberOfNotifications = Storage.getConfig(notificationConfig);
  let setNumberOfNotifications = notifications =>
    localStorage
    |> setItem(
         notificationConfig,
         Array.length(notifications) |> string_of_int,
       );

  let interval =
    (
      switch (localStorage |> getItem("github_interval")) {
      | Some("") => 60
      | Some(value) => int_of_string(value)
      | None => 60
      }
    )
    * 1000;
};

let getNotifications = () =>
  Config.hasConfig ?
    {
      let request =
        Axios.makeConfigWithUrl(
          ~url="https://api.github.com/notifications",
          ~_method="GET",
          ~headers={
            "Authorization": "Token " ++ Storage.getConfig("github"),
            "If-None-Match": "",
          },
          (),
        );

      Js.Promise.(
        Axios.request(request)
        |> then_(response => {
             let notifications = response##data |> Decode.decodeNotifications;
             Config.setNumberOfNotifications(notifications);
             notifications |> resolve;
           })
      );
    } :
    Js.Promise.resolve([||]);
