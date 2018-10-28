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
