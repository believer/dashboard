type attachmentType =
  | Image
  | UnknownAttachment;

type notificationType =
  | Added
  | AddedToOrganization
  | AddAttachment
  | CardDue
  | ChangeCard
  | Comment
  | Mentioned
  | Removed
  | UnknownType;

type trelloList = {name: string};
type trelloCard = {
  closed: option(bool),
  due: option(string),
  shortLink: string,
  name: string,
};

type creator = {fullName: string};

type trelloAttachment = {
  attachmentType,
  url: string,
};

type notificationData = {
  attachment: option(trelloAttachment),
  board: option(trelloList),
  card: option(trelloCard),
  text: option(string),
  listAfter: option(trelloList),
  listBefore: option(trelloList),
};

type notification = {
  creator: option(creator),
  id: string,
  unread: bool,
  date: string,
  data: notificationData,
  type_: notificationType,
};

module Decode = {
  open Json.Decode;

  let trelloList = json => {name: json |> field("name", string)};
  let trelloCard = json => {
    closed: json |> optional(field("closed", bool)),
    due: json |> optional(field("due", string)),
    shortLink: json |> field("shortLink", string),
    name: json |> field("name", string),
  };

  let trelloAttachment = json => {
    attachmentType:
      json
      |> field("url", string)
      |> Js.String.toLowerCase
      |> (
        field =>
          switch (
            field
            |> Js.String.substringToEnd(~from=Js.String.length(field) - 3)
          ) {
          | "png" => Image
          | _ => UnknownAttachment
          }
      ),
    url: json |> field("url", string),
  };

  let data = json => {
    attachment: json |> optional(field("attachment", trelloAttachment)),
    board: json |> optional(field("board", trelloList)),
    card: json |> optional(field("card", trelloCard)),
    text: json |> optional(field("text", string)),
    listAfter: json |> optional(field("listAfter", trelloList)),
    listBefore: json |> optional(field("listBefore", trelloList)),
  };

  let creator = json => {fullName: json |> field("fullName", string)};

  let parseNotifications = json => {
    id: json |> field("id", string),
    unread: json |> field("unread", bool),
    date: json |> field("date", string),
    data: json |> field("data", data),
    creator: json |> optional(field("memberCreator", creator)),
    type_:
      switch (json |> field("type", string)) {
      | "addAttachmentToCard" => AddAttachment
      | "addedToOrganization" => AddedToOrganization
      | "addedToCard" => Added
      | "cardDueSoon" => CardDue
      | "commentCard" => Comment
      | "changeCard" => ChangeCard
      | "mentionedOnCard" => Mentioned
      | "removedFromCard" => Removed
      | _ => UnknownType
      },
  };

  let decodeNotifications = json => json |> array(parseNotifications);
};

module Config = {
  open Dom.Storage;

  let notificationConfig = "trello_notifications";
  let hasConfig =
    Storage.hasConfig("trello_username")
    && Storage.hasConfig("trello_key")
    && Storage.hasConfig("trello_token");
  let numberOfNotifications = Storage.getConfig(notificationConfig);
  let setNumberOfNotifications = notifications =>
    localStorage
    |> setItem(
         notificationConfig,
         Array.length(notifications) |> string_of_int,
       );

  let interval =
    (
      switch (localStorage |> getItem("trello_interval")) {
      | Some("") => 60
      | Some(value) => int_of_string(value)
      | None => 60
      }
    )
    * 1000;
};

let trelloUrl = route =>
  "https://api.trello.com/1"
  ++ route
  ++ "/?key="
  ++ Storage.getConfig("trello_key")
  ++ "&token="
  ++ Storage.getConfig("trello_token");

let getNotifications = () =>
  Config.hasConfig ?
    {
      let request =
        Axios.makeConfigWithUrl(
          ~url=
            trelloUrl(
              "/members/"
              ++ Storage.getConfig("trello_username")
              ++ "/notifications",
            )
            ++ "&filter=all&read_filter=unread&fields=all&limit=50&page=0&memberCreator=true&memberCreator_fields=avatarHash%2CfullName%2Cinitials%2Cusername&",
          ~_method="GET",
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

let markNotificationAsRead = id => {
  let request =
    Axios.makeConfigWithUrl(
      ~url=trelloUrl("/notifications/" ++ id),
      ~_method="PUT",
      ~data={"unread": false},
      (),
    );

  Js.Promise.(
    Axios.request(request) |> then_(response => response |> resolve)
  );
};

let markAllNotificationsAsRead = () => {
  let request =
    Axios.makeConfigWithUrl(
      ~url=trelloUrl("/notifications/all/read"),
      ~_method="POST",
      (),
    );

  Js.Promise.(
    Axios.request(request) |> then_(response => response |> resolve)
  );
};
