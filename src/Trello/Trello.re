type notificationType =
  | Mentioned
  | UnknownType;

type creator = {fullName: string};

type notificationData = {text: string};

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

  let data = json => {text: json |> field("text", string)};

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
      | _ => UnknownType
      },
  };

  let decodeNotifications = json => json |> array(parseNotifications);
};

let getNotifications = () => {
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
    |> then_(response =>
         response##data |> Decode.decodeNotifications |> resolve
       )
  );
};
