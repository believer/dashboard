open Dom.Storage;

module SettingsForm = {
  open Formality;

  type field =
    | Interval
    | Username
    | Key
    | Token;

  type state = {
    fetchInterval: string,
    token: string,
    username: string,
    key: string,
  };

  type message = string;

  module TokenField = {
    let update = (state, value) => {...state, token: value};

    let validator = {
      field: Token,
      strategy: Strategy.OnFirstSuccessOrFirstBlur,
      dependents: None,
      validate: ({token}) =>
        switch (token) {
        | "" => Error("Token is required")
        | _ => Ok(Valid)
        },
    };
  };

  module UsernameField = {
    let update = (state, value) => {...state, username: value};

    let validator = {
      field: Username,
      strategy: Strategy.OnFirstSuccessOrFirstBlur,
      dependents: None,
      validate: ({username}) =>
        switch (username) {
        | "" => Error("Username is required")
        | _ => Ok(Valid)
        },
    };
  };

  module KeyField = {
    let update = (state, value) => {...state, key: value};

    let validator = {
      field: Key,
      strategy: Strategy.OnFirstSuccessOrFirstBlur,
      dependents: None,
      validate: ({key}) =>
        switch (key) {
        | "" => Error("Key is required")
        | _ => Ok(Valid)
        },
    };
  };

  module IntervalField = {
    let update = (state, value) => {...state, fetchInterval: value};

    let validator = {
      field: Interval,
      strategy: Strategy.OnFirstSuccessOrFirstBlur,
      dependents: None,
      validate: ({fetchInterval}) =>
        switch (fetchInterval) {
        | "" => Error("Token is required")
        | _ => Ok(Valid)
        },
    };
  };

  let validators = [
    TokenField.validator,
    UsernameField.validator,
    KeyField.validator,
    IntervalField.validator,
  ];
};

module SettingsFormContainer = Formality.Make(SettingsForm);

let component = ReasonReact.statelessComponent("TrelloSettings");

let make = _children => {
  ...component,
  render: _self =>
    <SettingsFormContainer
      initialState={
        fetchInterval: Trello.Config.interval / 1000 |> string_of_int,
        username: Storage.getConfig("trello_username"),
        key: Storage.getConfig("trello_key"),
        token: Storage.getConfig("trello_token"),
      }
      onSubmit={
        (state, form) => {
          localStorage |> setItem("trello_username", state.username);
          localStorage |> setItem("trello_key", state.key);
          localStorage |> setItem("trello_token", state.token);
          localStorage |> setItem("trello_interval", state.fetchInterval);

          form.notifyOnSuccess(None);
        }
      }>
      ...{
           form =>
             <form onSubmit={form.submit->Formality.Dom.preventDefault}>
               <h2 className="f3 mt0 dark-gray">
                 "Trello settings"->Utils.str
               </h2>
               <Input
                 error={SettingsForm.Username->(form.result)}
                 label="Username"
                 id="username"
                 onChange={
                   event =>
                     form.change(
                       SettingsForm.Username,
                       SettingsForm.UsernameField.update(
                         form.state,
                         event->ReactEvent.Form.target##value,
                       ),
                     )
                 }
                 value={form.state.username}
               />
               <Input
                 error={SettingsForm.Key->(form.result)}
                 label="Key"
                 id="key"
                 help={
                   Some([|
                     "Generate a key "->Utils.str,
                     <a
                       className="link dark-blue hover-hot-pink"
                       href="https://trello.com/app-key"
                       target="_blank">
                       {"here" |> Utils.str}
                     </a>,
                   |])
                 }
                 onChange={
                   event =>
                     form.change(
                       SettingsForm.Key,
                       SettingsForm.KeyField.update(
                         form.state,
                         event->ReactEvent.Form.target##value,
                       ),
                     )
                 }
                 value={form.state.key}
               />
               <Input
                 error={SettingsForm.Token->(form.result)}
                 label="Token"
                 id="token"
                 help={
                   Some([|
                     "Generate a token "->Utils.str,
                     <a
                       className="link dark-blue hover-hot-pink"
                       href="https://trello.com/app-key"
                       target="_blank">
                       "here"->Utils.str
                     </a>,
                   |])
                 }
                 onChange={
                   event =>
                     form.change(
                       SettingsForm.Token,
                       SettingsForm.TokenField.update(
                         form.state,
                         event->ReactEvent.Form.target##value,
                       ),
                     )
                 }
                 value={form.state.token}
               />
               <Input
                 error={SettingsForm.Interval->(form.result)}
                 label="Update interval (seconds)"
                 id="fetchInterval"
                 onChange={
                   event =>
                     form.change(
                       SettingsForm.Interval,
                       SettingsForm.IntervalField.update(
                         form.state,
                         event->ReactEvent.Form.target##value,
                       ),
                     )
                 }
                 value={form.state.fetchInterval}
               />
               {
                 switch (form.status) {
                 | Submitted =>
                   <SuccessMessage> "Saved"->Utils.str </SuccessMessage>
                 | _ => ReasonReact.null
                 }
               }
               <Button type_="submit"> "Save"->Utils.str </Button>
             </form>
         }
    </SettingsFormContainer>,
};
