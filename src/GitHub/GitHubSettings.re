open Dom.Storage;

module SettingsForm = {
  open Formality;

  type field =
    | Interval
    | Token;

  type state = {
    token: string,
    fetchInterval: string,
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

  let validators = [TokenField.validator, IntervalField.validator];
};

module SettingsFormContainer = Formality.Make(SettingsForm);

let component = ReasonReact.statelessComponent("GitHubSettings");

let make = _children => {
  ...component,
  render: _self =>
    <SettingsFormContainer
      initialState={
        fetchInterval: GitHub.Config.interval / 1000 |> string_of_int,
        token: Storage.getConfig("github"),
      }
      onSubmit={
        (state, form) => {
          localStorage |> setItem("github", state.token);
          localStorage |> setItem("github_interval", state.fetchInterval);

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
                 "GitHub settings"->Utils.str
               </h2>
               <Input
                 error={Token->(form.result)}
                 label="Token"
                 help={
                   Some([|
                     "Generate a token "->Utils.str,
                     <ExternalLink href="https://github.com/settings/tokens">
                       "here"->Utils.str
                     </ExternalLink>,
                     ". It needs permission "->Utils.str,
                     <strong> "notifications"->Utils.str </strong>,
                     " and if you want to see private repos it also needs permission "
                     ->Utils.str,
                     <strong> "repo"->Utils.str </strong>,
                     "."->Utils.str,
                   |])
                 }
                 id="token"
                 onChange={
                   handleOnChange(Token, SettingsForm.TokenField.update)
                 }
                 value={form.state.token}
               />
               <Input
                 error={SettingsForm.Token->(form.result)}
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
