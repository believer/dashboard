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
        fetchInterval: Storage.GitHub.config.interval / 1000 |> string_of_int,
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
           form =>
             <form onSubmit={form.submit->Formality.Dom.preventDefault}>
               <h2 className="f3 mt0 dark-gray">
                 "GitHub settings"->Utils.str
               </h2>
               <Input
                 error={SettingsForm.Token->(form.result)}
                 label="Token"
                 help={
                   Some([|
                     "Generate a token "->Utils.str,
                     <a
                       className="link blue hover-hot-pink"
                       href="https://github.com/settings/tokens"
                       target="_blank">
                       "here"->Utils.str
                     </a>,
                   |])
                 }
                 id="token"
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
                 error={SettingsForm.Token->(form.result)}
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
                   <div className="green mb2"> "Saved"->Utils.str </div>
                 | _ => ReasonReact.null
                 }
               }
               <Button type_="submit"> "Save"->Utils.str </Button>
             </form>
         }
    </SettingsFormContainer>,
};
