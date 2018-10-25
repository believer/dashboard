type notificationType =
  | Added
  | AddedToOrganization
  | Mentioned
  | Comment
  | ChangeCard
  | Removed
  | UnknownType;

type trelloList = {name: string};
type trelloCard = {
  shortLink: string,
  name: string,
};

type creator = {fullName: string};

type notificationData = {
  board: option(trelloList),
  card: option(trelloCard),
  text: option(string),
  listAfter: option(trelloList),
  listBefore: option(trelloList),
};

type notification = {
  creator,
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
    shortLink: json |> field("shortLink", string),
    name: json |> field("name", string),
  };

  let data = json => {
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
    creator: json |> field("memberCreator", creator),
    type_:
      switch (json |> field("type", string)) {
      | "addedToOrganization" => AddedToOrganization
      | "addedToCard" => Added
      | "mentionedOnCard" => Mentioned
      | "removedFromCard" => Removed
      | "commentCard" => Comment
      | "changeCard" => ChangeCard
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

let getNotifications = () =>
  Config.hasConfig ?
    {
      let request =
        Axios.makeConfigWithUrl(
          ~url=
            "https://api.trello.com/1/members/"
            ++ Storage.getConfig("trello_username")
            ++ "/notifications?filter=all&read_filter=unread&fields=all&limit=50&page=0&memberCreator=true&memberCreator_fields=avatarHash%2CfullName%2Cinitials%2Cusername&key="
            ++ Storage.getConfig("trello_key")
            ++ "&token="
            ++ Storage.getConfig("trello_token"),
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
