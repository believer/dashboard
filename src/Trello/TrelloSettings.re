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

let generateHelp = text =>
  Some([|
    text->Utils.str,
    <ExternalLink href="https://trello.com/app-key">
      "here"->Utils.str
    </ExternalLink>,
  |]);

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
           form => {
             let handleOnChange = (field, updater, event) =>
               form.change(
                 field,
                 updater(form.state, event->ReactEvent.Form.target##value),
               );

             <form onSubmit={form.submit->Formality.Dom.preventDefault}>
               <h2 className="f3 mt0 dark-gray">
                 "Trello settings"->Utils.str
               </h2>
               <Input
                 error={Username->(form.result)}
                 label="Username"
                 id="username"
                 onChange={
                   handleOnChange(Username, SettingsForm.UsernameField.update)
                 }
                 value={form.state.username}
               />
               <Input
                 error={Key->(form.result)}
                 label="Key"
                 id="key"
                 help={generateHelp("Generate a key ")}
                 onChange={handleOnChange(Key, SettingsForm.KeyField.update)}
                 value={form.state.key}
               />
               <Input
                 error={Token->(form.result)}
                 label="Token"
                 id="token"
                 help={generateHelp("Generate a token ")}
                 onChange={
                   handleOnChange(Token, SettingsForm.TokenField.update)
                 }
                 value={form.state.token}
               />
               <Input
                 error={Interval->(form.result)}
                 label="Update interval (seconds)"
                 id="fetchInterval"
                 onChange={
                   handleOnChange(Interval, SettingsForm.IntervalField.update)
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
             </form>;
           }
         }
    </SettingsFormContainer>,
};