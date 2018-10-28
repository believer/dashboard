module Config = {
  open Dom.Storage;

  let notificationConfig = "github_notifications";

  let hasConfig = Storage.hasConfig("github");
  let numberOfNotifications = Storage.getConfig(notificationConfig);
  let setNumberOfNotifications = notifications =>
    localStorage
    |> setItem(
         notificationConfig,
         Belt.Array.length(notifications) |> string_of_int,
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
