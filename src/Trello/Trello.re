type notificationType =
  | Mentioned
  | Comment
  | ChangeCard
  | UnknownType;

type trelloList = {name: string};
type trelloCard = {shortLink: string};

type creator = {fullName: string};

type notificationData = {
  board: trelloList,
  card: trelloCard,
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
  let trelloCard = json => {shortLink: json |> field("shortLink", string)};

  let data = json => {
    board: json |> field("board", trelloList),
    card: json |> field("card", trelloCard),
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
      | "mentionedOnCard" => Mentioned
      | "commentCard" => Comment
      | "changeCard" => ChangeCard
      | _ => UnknownType
      },
  };

  let decodeNotifications = json => json |> array(parseNotifications);
};

module Config = {
  open Dom.Storage;

  let hasConfig =
    Storage.hasConfig("trello_username")
    && Storage.hasConfig("trello_key")
    && Storage.hasConfig("trello_token");

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

let getNotifications = () => {
  switch (Config.hasConfig) {
  | true => {let request =
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
    |> then_(response =>
         response##data |> Decode.decodeNotifications |> resolve
       )
  )
  }
  | false => Js.Promise.resolve([||])
  }
};
